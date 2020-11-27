#include <stdio.h>
#include <curl/curl.h>

int main(int argc, char ** argv) {
    CURL *curl;
    FILE *fp;

    fp = fopen(argv[2], "wb");

    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, argv[1]);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);

    int success = curl_easy_perform(curl);
    if (success == CURLE_OK) {
        printf("File downloaded!\n");
    } else {
        printf("ERROR: %s\n", curl_easy_strerror(success));
    }

    fclose(fp);
    curl_easy_cleanup(curl);

    return 0;
}