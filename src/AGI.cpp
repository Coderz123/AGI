#include "../include/AGI.h"
#include <crow.h>

bool debug = true;
std::vector<Emotion> emotions;

using namespace rapidjson;

int main (int argc, char** argv)
{
	//setupEmotions();


	std::ifstream ifs("data/Soul/EmotionData.json");
	IStreamWrapper isw(ifs);


	Document d;
	d.ParseStream(isw);

	assert(d.IsObject());
	assert(d.HasMember("Categories"));
	assert(d["Categories"].IsArray());
	const Value& a = d["Categories"];
	assert(a.IsArray());

    //for (SizeType i = 0; i < a.Size(); i++) // Uses SizeType instead of size_t
        //        printf("a[%d] = %d\n", i, a[i].GetInt());
    static const char* kTypeNames[] =
        { "Null", "False", "True", "Object", "Array", "String", "Number" };

    for (Value::ConstMemberIterator itr = d.MemberBegin();
        itr != d.MemberEnd(); ++itr)
    {
        printf("Type of member %s is %s\n",
            itr->name.GetString(), kTypeNames[itr->value.GetType()]);
    }

    for (auto& em : a.GetArray())
    {
        assert(em.IsObject());
        printf("Processing %s...\n", em["name"].GetString());
        int emId = em["id"].GetInt();
        std::string emName = em["name"].GetString();
        std::string emDescr = em["description"].GetString();
        int emOppId = em["oppositeId"].GetInt();
        const Value& nbrs = em["neighbours"].GetArray();
        std::vector<int> emNbrs{nbrs[0].GetInt(), nbrs[1].GetInt()};
        const Value& emns = em["emotions"].GetArray();

        for (auto& emObj : emns.GetArray())
        {
            std::string emotionName = emObj["emotion"].GetString();
            int emmin = emObj["min"].GetInt();
            int emmax = emObj["max"].GetInt();
            printf("\t %s [%d:%d]...\n", emotionName.c_str(), emmin, emmax);
        }

    }

	crow::SimpleApp app;
	crow::mustache::set_base(".");

	CROW_ROUTE(app, "/")([](const crow::request&, crow::response& res) {
        //replace cat.jpg with your file path
        res.set_static_file_info("templates/index.html");
        res.end();
    });

	CROW_ROUTE(app, "/actions")([&](){
		crow::mustache::context ctx;
		ctx["actions"] = actions;
		return crow::mustache::load("actions.html").render(ctx);

	});

	CROW_ROUTE(app, "/js/htmx.min.js")([&](const crow::request&, crow::response& res){
        res.set_static_file_info("js/htmx.min.js");
        res.end();
	});

	CROW_ROUTE(app, "/clicked")([&](){
		return "<p>This is the replaced text!</p>";

	});

	CROW_ROUTE(app, "/emotions")([&](){
		crow::mustache::context ctx;
		std::vector<std::string> descr;
		for (Emotion& em : emotions)
        {
            descr.push_back(em.toHtml());
        }
		ctx["emotions"] = descr;
		return crow::mustache::load("emotions.html").render(ctx);

	});


	auto _a = app.bindaddr("127.0.0.1")
	   .port(18080)
	   .multithreaded()
	   .run_async();
	return 0;

}
