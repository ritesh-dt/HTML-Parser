#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
using namespace std;

#include "main.cpp"

#define MAX_NODES_ALLOWED 20

// Constants
const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 480;

const int NODE_HEIGHT = 30;
const int NODE_X_SPACING = 10;
const int NODE_Y_SPACING = 50;
const int NODE_PADDING = 30;

// Colors
SDL_Color BLACK = {0, 0, 0};

// Globals
SDL_Renderer* renderer = NULL;
TTF_Font* font = NULL;
int scrollX, scrollY;
int nodeCount = 0;


// Classes
class RenderNode {
private:
	int x, y;
	int w, h;
	SDL_Texture* texture;
	SDL_Surface* textSurface;

public:
	string name;
	RenderNode* parent;

	RenderNode(string n, RenderNode* parentNode, int xPos, int yPos) {
		name = n;
		x = xPos;		y = yPos;
		w = name.length()*5 + NODE_PADDING; //30;
		h = NODE_HEIGHT;
		parent = parentNode;
		
		// Generate the text texture
		texture = NULL;

		// textSurface = TTF_RenderText_Solid(font, n.c_str(), BLACK);
		// if (textSurface == NULL) {
		// 	cout << name << " TTF_Error: " << TTF_GetError() << endl;
		// } else {
		// 	texture = SDL_CreateTextureFromSurface(renderer, textSurface);
		// 	if (texture == NULL) {
		// 		cout << name << " SDL Error: " << SDL_GetError() << endl;
		// 	}
		// 	SDL_FreeSurface(textSurface);
		// }	
	};
	render();
	setPos(int xPos, int yPos) {
		x = xPos;	y = yPos;
	}
	getWidth() {
		return w;
	}
	getAnchorX() {
		return x + w/2;
	}
	getAnchorY() {
		return y + h;
	}
	free() {
		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(texture);
		textSurface = NULL;
		texture = NULL;
	}
};

// Functions
bool init(SDL_Window*, SDL_Surface*);
void close(SDL_Window*, SDL_Surface*, SDL_Renderer*);

void postOrder(Node, RenderNode*, HashTable, RenderNode*);
void addNodeToList(RenderNode, RenderNode*);


int main(int argc, char* args[]) {
	SDL_Window* window = NULL;
	SDL_Surface* screenSurface = NULL;

    HashTable adjList = HashTable(151);
    parseHtml("example.html", &adjList);
    // adjList.displayHashTill(20);
    printTree(adjList, 5);
    
    RenderNode* nodeList = (RenderNode*) malloc(sizeof(RenderNode) * MAX_NODES_ALLOWED);
	if (!init(window, screenSurface))
		return -1;

	list<Node> forest = adjList.getItem(0);
	list<Node>::iterator iter = forest.begin();
	list<Node>::iterator end = forest.end();
	for (; iter != end; iter++) {
		cout << iter->name << endl;
		Node root = *iter;
    	postOrder(root, NULL, adjList, nodeList);
	}

	// Scrolling behaviour
	scrollX = 0;
	scrollY = 0;

	SDL_Surface* textOnScreen;
	if (textOnScreen != NULL) 	SDL_FreeSurface(textOnScreen);
	textOnScreen = TTF_RenderText_Solid(font, "HTML Parser", BLACK);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textOnScreen);
	if (textOnScreen != NULL)	SDL_FreeSurface(textOnScreen);

	bool isRunning = true;
	SDL_Event e;
	while (isRunning) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT)		isRunning = false;
			else if (e.type == SDL_KEYDOWN) {
				int key = e.key.keysym.sym;
				if (key == SDLK_UP)		scrollY += 5;
				if (key == SDLK_DOWN)		scrollY -= 5;
				if (key == SDLK_RIGHT)		scrollX += 5;
				if (key == SDLK_LEFT)		scrollX -= 5;
				switch (e.key.keysym.sym) {
					default:
						break;
				}
			}
		}

		SDL_SetRenderDrawColor(renderer, 0xEE, 0xEE, 0xEE, 0xFF);
		SDL_RenderClear(renderer);

		SDL_Rect nodeRect = {100+scrollY, 100+scrollX, 100, 35};
		SDL_RenderCopy(renderer, texture, NULL, &nodeRect);

		for (int idx=0; idx<nodeCount; idx++) {
			nodeList[idx].render();
		}

		SDL_RenderPresent(renderer);
	}
	SDL_DestroyTexture(texture);
	for (int idx=0; idx<nodeCount; idx++) {
		nodeList[idx].free();
	}
	close(window, screenSurface, renderer);
	return 0;
}

bool init(SDL_Window* window, SDL_Surface* screenSurface) {
	bool success = true;
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		cout << "Could not initialize SDL! Error: " << SDL_GetError() << endl;
		success = false;
	} else {
		window = SDL_CreateWindow("HTML Parser", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL) {
			cout << "Could not create window! Error: " << SDL_GetError() << endl;
			success = false;
		} else {
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			if (renderer == NULL) {
				cout << "Could not create renderer! Error: " << SDL_GetError() << endl;
				success = false;
			} else {
				if (TTF_Init() == -1) {
					cout << "Could not initialize SDL_ttf! Error: " << TTF_GetError() << endl;
					success = false;
				} else {
					font = TTF_OpenFont("Roboto-Regular.ttf", 28);
					if (font == NULL) {
						cout << "Could not open Roboto-Regular.tff! Error: " << TTF_GetError() << endl;
						success = false;
					}
				}
			}
			screenSurface = SDL_GetWindowSurface(window);
		}
	}
	return success;
}

void close(SDL_Window* window, SDL_Surface* screenSurface, SDL_Renderer* renderer){
	TTF_CloseFont(font);

	SDL_DestroyRenderer(renderer);
	renderer = NULL;

	SDL_DestroyWindow(window);
	screenSurface = NULL;
	window = NULL;

	TTF_Quit();
	SDL_Quit();
}

int nodeX = 10;
int nodeY = 30;

void postOrder(Node n, RenderNode* parentNode, HashTable hash, RenderNode* nodeList) {
	list<Node> elements = hash.getItem(n.id);
	int firstChildX = nodeX;
	
	RenderNode* newNode = (RenderNode*) malloc(sizeof(RenderNode));
    *newNode = RenderNode(n.name, parentNode, (firstChildX + nodeX)/2, nodeY);
    
	if (elements.size() > 2) {
        // Start getting child nodes from position 3 onwards
	   	list<Node>::iterator iter = elements.begin();
	    list<Node>::iterator end = elements.end();

	    nodeY += NODE_Y_SPACING;
	    for (advance(iter, 2); iter != end; iter++) {
	        postOrder(*iter, newNode, hash, nodeList);
	    }
	    nodeY -= NODE_Y_SPACING;
	    
	} else {
		nodeX += newNode->getWidth() + NODE_X_SPACING;
	}
	
   	newNode->setPos((firstChildX + nodeX)/2, nodeY);
   	addNodeToList(*newNode, nodeList);
    
    return;
}

void addNodeToList(RenderNode n, RenderNode* nodeList) {
	nodeList[nodeCount] = n;
	nodeCount++;
}

// Class methods
RenderNode::render() {
	SDL_Rect nodeRect = {x + scrollX, y + scrollY, w, h};
	SDL_SetRenderDrawColor(renderer, 0xBB, 0xBB, 0xBB, 0xFF);
	SDL_RenderFillRect(renderer, &nodeRect);

	// cout << name << textureWidth << " " << textureHeight << endl;
	// SDL_Rect textRect = {x + scrollX + (w - (w - NODE_PADDING))/2, y + (h - (h - NODE_PADDING))/2, w - NODE_PADDING, h - NODE_PADDING};
	// if (texture != NULL)	SDL_RenderCopy(renderer, texture, NULL, &nodeRect);
	// cout << name << " Error (rendering): " << TTF_GetError() << endl;

	// Draw edge
	if (parent != NULL)
		SDL_RenderDrawLine(renderer, x + w/2 + scrollX, y + scrollY, parent->getAnchorX() + scrollX, parent->getAnchorY() + scrollY);
}