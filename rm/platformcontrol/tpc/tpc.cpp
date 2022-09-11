#include <iostream>
#include <map>
#include <algorithm>
#include <cstdlib>
#include "cgroupcontrol.h"
#include "cpucontrol.h"
#include "iocontrol.h"
#include "cpusetcontrol.h"
#include "memorycontrol.h"
#include "cgrouputil.h"
#include "numericvalue.h"
#include "pcexception.h"

using namespace std;
using namespace pc;

static void checkAppDir(int pid)
{
    string realDir = pc::util::findCgroupPath(static_cast<pid_t>(pid));
    string wantedDir = pc::util::getCgroupAppBaseDir(static_cast<pid_t>(pid));
    if (realDir != wantedDir) {
        cerr << "checkAppDir: invalid app base dir\n";
        exit(EXIT_FAILURE);
    }
    cout << pid << " base directory is " << realDir << endl;
}

static void setCpuMax(std::shared_ptr<App> app, int n)
{
    //pc::CpuControl().setValue(pc::CpuControl::MAX, n, app);
    pc::CpuControl().setCpuMax(n, app);
    pc::NumericValue nv = pc::CpuControl().getCpuMax(app);
    cout << "Cpu max: requested " << n << " read " << nv << endl;
    if (nv != n)
        cout << "ERROR setCpuMax\n";
    else
        cout << "setCpuMax OK\n";
}

static void setCpuMax(std::shared_ptr<App> app)
{
    pc::CpuControl().setCpuMax(pc::NumericValue::max(), app);
    pc::NumericValue nv = pc::CpuControl().getCpuMax(app);
    cout << "Cpu max: requested " << pc::NumericValue::max() << " read " << nv << endl;
    if (!nv.isMax())
        cout << "ERROR setCpuMax(max)\n";
    else
        cout << "setCpuMax(max) OK\n";
}

static void getCpuStat(std::shared_ptr<App> app)
{
    map<string, uint64_t> tags = pc::CpuControl().getCpuStat(app);
    cout << "CPU STAT\n";
    for (const auto& kv : tags) {
        cout << "    " << kv.first << ":" << kv.second << endl;
    }
}

static void getIoMax(std::shared_ptr<App> app, int major, int minor)
{
    cout << "Setting max wbps\n";
    pc::IOControl().setIOMax(8, 0, pc::IOControl::WBPS, 1000000, app);
    sleep(2);
    map<string, pc::NumericValue> tags = pc::IOControl().getIOMax(major, minor, app);
    cout << "IO MAX\n";
    for (const auto& kv : tags) {
        cout << "    " << kv.first << ":" << kv.second << endl;
    }
    if (tags["wbps"] != 1000000)
        cout << "ERROR getIoMax\n";
    else
        cout << "getIoMax OK\n";
}

static void getIoStat(std::shared_ptr<App> app, int major, int minor)
{
    map<string, pc::NumericValue> tags = pc::IOControl().getIOStat(major, minor, app);
    cout << "IO STAT\n";
    for (const auto& kv : tags) {
        cout << kv.first << ":" << kv.second << endl;
    }
}

static void setCpuSet(std::shared_ptr<App> app)
{
    pc::CpusetControl().setCpusetCpus({ {0, 0}, {3, 3} }, app);
    pc::CpusetControl::CpusetVector cpus = pc::CpusetControl().getCpusetCpus(app);
    if (std::find(begin(cpus), end(cpus), std::make_pair((short)0, (short)0)) == cpus.end() ||
            std::find(begin(cpus), end(cpus), std::make_pair((short)3, (short)3)) == cpus.end()) {
        cout << "ERROR setCpuSet\n";
    } else {
        cout << "setCpuSet OK\n";
    }
}

static void testMemoryControl(std::shared_ptr<App> app)
{
    pc::MemoryControl().setMemoryMin(16384, app);
    int mmin = pc::MemoryControl().getMemoryMin(app);
    if (mmin != 16384) {
        cout << "ERROR testMemoryControl setMemoryMin. Requested 16384, got " << mmin << "\n";
        return;
    }

    pc::MemoryControl().setMemoryMax(98304, app);
    int mmax = pc::MemoryControl().getMemoryMax(app);
    if (pc::MemoryControl().getMemoryMax(app) != 98304) {
        cout << "ERROR testMemoryControl setMemoryMax. Requested 100000, got " << mmax << "\n";
        return;
    }

    map<string, uint64_t> events = pc::MemoryControl().getMemoryEvents(app);
    cout << "MEMORY EVENTS\n";
    for (const auto &ev: events) {
        cout << "    " << ev.first << ' ' << ev.second << endl;
    }
    cout << "testMemoryControl OK\n";
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        cerr << "Missing parameter: PID\n";
        exit(EXIT_FAILURE);
    }
    int pid = atoi(argv[1]);

    pc::CGroupControl cgc;
    std::shared_ptr<App> app = App::makeApp(pid, pc::App::STANDALONE);

    int major = 8, minor = 0;
    try {
        cgc.addApplication(app);
        checkAppDir(pid);
        testMemoryControl(app);
        setCpuMax(app, 33);
        setCpuMax(app);
        getCpuStat(app);
        setCpuSet(app);
        getIoStat(app, major, minor);
        getIoMax(app, major, minor);
    } catch (pc::PcException &e) {
        cerr << "PcException: " << e.what() << endl;
        exit(EXIT_FAILURE);
    }

}