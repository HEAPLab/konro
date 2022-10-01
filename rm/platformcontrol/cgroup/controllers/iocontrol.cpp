#include "iocontrol.h"
#include "../utilities/tsplit.h"
#include "../utilities/keyvalueparser.h"
#include <vector>
#include <sstream>
#include <iostream>

using namespace std;

namespace pc {

/*static*/
const char *IOControl::controllerName_ = "io";

/*static*/
const map<IOControl::ControllerFile, const char *> IOControl::fileNamesMap_ = {
    { STAT, "io.stat" },
    { MAX, "io.max" }
};

/*static*/
const map<IOControl::IoMax, const char *> IOControl::keyNames_ = {
    { RBPS, "rbps" },
    { WBPS, "wbps" },
    { RIOPS, "riops" },
    { WIOPS, "wiops" },
};

std::map<string, NumericValue> IOControl::getIOHelper(ControllerFile cf, int major, int minor, std::shared_ptr<App> app)
{
    vector<string> lines = getContent(controllerName_, fileNamesMap_.at(cf), app);
    map<string, NumericValue> tags;
    for (auto &line: lines) {
        tags = KeyValueParser().parseLineNv(line, major, minor);
        if (!tags.empty())
            break;
    }
    return tags;
}

IOControl &IOControl::instance()
{
    static IOControl ioc;
    return ioc;
}

void IOControl::setMax(int major, int minor, IoMax ioMax, NumericValue value, std::shared_ptr<App> app)
{
    ostringstream os;
    os << major << ':' << minor << ' ' << keyNames_.at(ioMax) << '=' << value;;
    setValue(controllerName_, fileNamesMap_.at(MAX), os.str(), app);
}

}   // namespace pc
