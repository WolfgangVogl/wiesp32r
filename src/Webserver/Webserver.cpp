#include "Webserver.hpp"

#include "arduino.hpp"
#include "global.hpp"
#include "serial.hpp"

#include <ESPAsyncWebServer.h>

namespace wv::wiesp {

namespace {
static const char* style = "";
static const char* nav = "";
} // namespace

struct Webserver::Private
{
    AsyncWebServer server = AsyncWebServer(80);
    static String processor(const String& var)
    {
        logger.println("[HTTP] process template var '", var, "'");

        if (var == "TMPL_SIDEBAR") {
            return String(random(10, 20));
        }

        else if (var == "TMPL_MAIN") {
            return String(random(0, 50));
        }

        return String();
    }

    Private()
    {
        using RQ = AsyncWebServerRequest;
        /*
        server.on("/", [](RQ* request) {
            AsyncWebServerResponse* response = request->beginResponse(Serial, "text/html", 12);
            response->setCode(200);
            response->setContentLength(12);
            response->addHeader("Server", "ESP Async Web Server");
            request->send(response);
        });
        */

        server.on("/", [](RQ* request) {
            request->send(200, "text/html", R"(
<!DOCTYPE html><head>
<title>wiesp32A - Wiegang Access Control</title>
<link rel="stylesheet" href="style.css">
</head>
<body>
<content>
<h1>Wiegand Access Control</h1>
<sidebar>%TMPL_SIDEBAR%</sidebar>
<main>%TMPL_MAIN%</main>
</content>
</body>
</html>
)");
        });
        server.on("/style.css", [](RQ* request) {
            request->send(200, "text/html", R"(

)");
        });
        server.on("/sidebar", [](RQ* request) {
            request->send(200, "text/html", R"(
<!DOCTYPE html>
<body><ul>
<li><a href="/">Home</a></li>
<li><a href="/content">Content</a></li>
<li><a href="/heap">Heap</a></li>
</ul></body>
)");
        });
        server.on("/content", [](RQ* request) {
            request->send(200, "text/html", R"(
<!DOCTYPE html>
<body>
<p>This is the content</p>
<p>Free heap: <span id="heap"></span></p>
<p>1 Bla bla bla</p>
<p>2 Bla bla bla</p>
<p>3 Bla bla bla</p>
<p>4 Bla bla bla</p>
<p>5 Bla bla bla</p>
</body>
)");
        });

        server.on("/content", [](RQ* request) {
            request->send(200, "text/html", R"(
<!DOCTYPE html>
<body><h1>Content</h1></body>
)");
        });

        server.on("/heap", HTTP_GET, [](RQ* request) {
            logger.println("[HTTP] GET /heap");
            request->send(200, "text/plain", String(ESP.getFreeHeap()));
        });

        server.onNotFound([](RQ* request) {
            logger.print("[HTTP] ");
            logger.print(request->method());
            logger.print(" ");
            logger.print(request->url());
            logger.println(" => 404 (NOT FOUND)");
            request->send(404);
        });

        server.begin();
    }
};

void Webserver::run()
{
    if (!mImpl) {
        mImpl = new Private();
        logger.println("Starting Webserver");
    }
}
Webserver::~Webserver()
{
    if (mImpl) {
        delete mImpl;
    }
}

} // namespace wv::wiesp