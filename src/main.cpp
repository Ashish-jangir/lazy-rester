#include <curl/curl.h>
#include <curl/easy.h>

int main() {
  curl_global_init(CURL_GLOBAL_DEFAULT);

  CURL *curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_URL,
                   "https://pokeapi.co/api/v2/pokemon/ditto");
  curl_easy_perform(curl);

  if (curl) {
    curl_easy_cleanup(curl);
  }

  curl_global_cleanup();
}
