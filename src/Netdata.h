#define CPU_CHART_ID "system.cpu"
#define MEM_CHART_ID "system.ram"
#define DISK_CHART_ID "system.io"
#include <ArduinoJson.h>
#include<HTTPClient.h>
const char* HOST = "http://10.0.0.1:19999";


class NetChartData
{
public:
    int api;
    String id;
    String name;

    int view_update_every;
    int update_every;
    long first_entry;
    long last_entry;
    long before;
    long after;
    String group;
    String options_0;
    String options_1;

    JsonArray dimension_names;
    JsonArray dimension_ids;
    JsonArray latest_values;
    JsonArray view_latest_values;
    int dimensions;
    int points;
    String format;
    JsonArray result;
    double min;
    double max;
};

void parseNetDataResponse(JsonDocument json, NetChartData& chartData) {
    chartData.api = json["api"];
    chartData.id = json["id"].as<String>();
    chartData.name = json["name"].as<String>();
    chartData.view_update_every = json["view_update_every"]; // 1
    chartData.update_every = json["update_every"]; 
    chartData.first_entry = json["first_entry"];
    chartData.last_entry = json["last_entry"];
    chartData.before = json["before"];
    chartData.after = json["after"];
    chartData.group = json["group"].as<String>();
    chartData.options_0 = json["options"][0].as<String>();
    chartData.options_1 = json["options"][1].as<String>();
    chartData.dimension_names = json["dimension_names"];
    chartData.dimension_ids = json["dimension_ids"];
    chartData.latest_values = json["latest_values"];
    chartData.view_latest_values = json["view_latest_values"];
    chartData.dimensions = json["dimensions"];
    chartData.points = json["points"];
    chartData.format = json["format"].as<String>();
    chartData.result = json["result"];
    chartData.min = json["min"];
    chartData.max = json["max"];

    JsonArray db_points_per_tier = json["db_points_per_tier"];
}


bool getNetdataWithdimension(String charId, NetChartData& chartData, String dimensions_filter) {
    String reqArgs = "/api/v1/data?chart=" + charId + "&format=json&points=1&group=average&gtime=0&options=s%7Cjsonwrap%7Cnonzero&after=-2&dimensions=" + dimensions_filter;
    String reqURI = HOST + reqArgs;
    JsonDocument chartDataJson;
    HTTPClient http;
    http.begin(reqURI);
    int httpCode = http.GET();
    if (httpCode == 200) {
        String payload = http.getString();
        deserializeJson(chartDataJson, payload);
        parseNetDataResponse(chartDataJson, chartData);
        Serial.println(chartDataJson["max"].as<double>());
        return true;
    }
    return false;
}

