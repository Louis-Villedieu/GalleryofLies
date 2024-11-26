#include "../include/npc.hpp"
#include <curl/curl.h>
#include <nlohmann/json.hpp>

NPC::NPC(std::string firstName, std::string lastName, std::string aiOrder, int positionX, int positionY, SDL_Texture* texture, SDL_Renderer* renderer) : Character(firstName, lastName, aiOrder, positionX, positionY, texture, renderer) {}

NPC::~NPC() {}

void NPC::renderSprite() {
    SDL_Rect srcRect = {currentFrame * frameWidth, currentRow * frameHeight, frameWidth, frameHeight};
    SDL_Rect destRect = {positionX, positionY, frameWidth / 3, frameHeight / 3};
    SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
}

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string NPC::thinkAndAnswer(std::string question) {
    CURL* curl = curl_easy_init();
    std::string response;
    if(curl) {
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "Authorization: Bearer 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");
        
        nlohmann::json requestData = {
            {"model", "gpt-3.5-turbo"},
            {"messages", {{
                {"role", "user"},
                {"content", "Your are a npc in a game named " + firstName + " " + lastName + ". " + "Your role is " + aiOrder 
                + ". " + "The player asked you: " + question + " and you must answer in a short sentence. Here is your past conversation: " }  //TODO: add the past conversation
            }}}
        };
        std::string jsonString = requestData.dump();
        
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/chat/completions");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonString.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        
        CURLcode res = curl_easy_perform(curl);
        
        if(res != CURLE_OK) {
            response = "Erreur lors de la requête: " + std::string(curl_easy_strerror(res));
        } else {
            try {
                auto jsonResponse = nlohmann::json::parse(response);
                response = jsonResponse["choices"][0]["message"]["content"];
            } catch (const std::exception& e) {
                response = "Erreur lors du parsing de la réponse: " + std::string(e.what());
            }
        }
        
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    
    return response;
}

void NPC::setTexture(SDL_Texture* texture) {
    this->texture = texture;
}