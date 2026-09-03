#include "Body/Action/verbs.h"
#include "Soul/Emotion/EmotionData.h"

#include <crow.h>

#include <exception>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#ifndef AGI_SOURCE_DIR
#define AGI_SOURCE_DIR "."
#endif

int main()
{
    const std::filesystem::path sourceRoot{AGI_SOURCE_DIR};
    std::vector<Emotion> emotions;

    try
    {
        emotions = loadEmotions(sourceRoot / "data/Soul/EmotionData.json");
    }
    catch (const std::exception& error)
    {
        std::cerr << "Failed to start AGI: " << error.what() << '\n';
        return 1;
    }

    std::cout << "Loaded " << emotions.size() << " emotion categories.\n";

    crow::SimpleApp app;
    crow::mustache::set_global_base((sourceRoot / "templates").string());

    CROW_ROUTE(app, "/")([sourceRoot](const crow::request&, crow::response& response) {
        response.set_static_file_info_unsafe((sourceRoot / "templates/index.html").string());
        response.end();
    });

    CROW_ROUTE(app, "/actions")([] {
        crow::mustache::context context;
        context["actions"] = actions;
        return crow::mustache::load("actions.html").render(context);
    });

    CROW_ROUTE(app, "/js/htmx.min.js")(
        [sourceRoot](const crow::request&, crow::response& response) {
            response.set_static_file_info_unsafe((sourceRoot / "js/htmx.min.js").string());
            response.end();
        });

    CROW_ROUTE(app, "/clicked")([] {
        return "<p>This is the replaced text!</p>";
    });

    CROW_ROUTE(app, "/emotions")([&emotions] {
        crow::mustache::context context;
        std::vector<std::string> descriptions;
        descriptions.reserve(emotions.size());
        for (const auto& emotion : emotions)
        {
            descriptions.push_back(emotion.toString());
        }
        context["emotions"] = descriptions;
        return crow::mustache::load("emotions.html").render(context);
    });

    app.bindaddr("127.0.0.1").port(18080).multithreaded().run();
}
