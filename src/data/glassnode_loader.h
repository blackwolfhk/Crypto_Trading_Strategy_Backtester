#pragma once
#include "data_loader.h"
#include <string>
#include <curl/curl.h>
#include <ctime>

class GlassnodeLoader {
private:
    std::string api_key;
    
    // Callback function for curl
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
        userp->append((char*)contents, size * nmemb);
        return size * nmemb;
    }
    
public:
    GlassnodeLoader(const std::string& key) : api_key(key) {}
    
    std::vector<OHLCV> fetchBTCData(const std::string& start_date, const std::string& end_date) {
        std::vector<OHLCV> data;
        CURL* curl;
        CURLcode res;
        std::string readBuffer;
        
        curl = curl_easy_init();
        if(curl) {
            // Format the URL with your API key
            std::string url = "https://api.glassnode.com/v1/metrics/market/ohlc";
            url += "?api_key=" + api_key;
            url += "&a=BTC&i=1d"; // Bitcoin, 1 day interval
            url += "&s=" + start_date; // e.g., "2020-01-01"
            url += "&u=" + end_date;   // e.g., "2023-05-01"
            
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            
            // Perform the request
            res = curl_easy_perform(curl);
            
            // Check for errors
            if(res != CURLE_OK) {
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            } else {
                // Parse the JSON response to OHLCV data
                // Note: You'll need to add a JSON parsing library
                // This is a placeholder for where you'd parse JSON
                // ...
                
                std::cout << "Data received from Glassnode API\n";
            }
            
            curl_easy_cleanup(curl);
        }
        
        return data;
    }
};
