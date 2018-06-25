#include <vector>
#include <experimental/optional>
#include <iostream>
#include <unordered_map>
#include "dsl/programloader.h"
#include "dsl/interpreter.h"
#include "dsl/utils.h"
#include "device_manager.h"

using namespace dsl;
using namespace std;

namespace dm {

    device_manager::device_manager(string fileName) {
        prog = load_program(fileName);
    }

    device_manager::~device_manager() {
        for (pair<int, dev_cluster*> DC_cluster : DC_clusters) {
            if (DC_cluster.second != nullptr)
                delete DC_cluster.second;
        }

        for (pair<string, vector< pair<int, dev_cluster*> >* > context_cluster : context_clusters) {
            if (context_cluster.second == nullptr)
                continue;
            
            for (pair<int, dev_cluster*> device_clust : *context_cluster.second) {
                if (device_clust.second != nullptr)
                    delete device_clust.second;
            }
            delete context_cluster.second;
        }
    }

    Input device_manager::getInput(string line) {
        Input input;

        string numStr;
        bool listMode = false;
        vector<int> list;
        for (char c : line) {
            if (listMode) {
                if (c == ',') {
                    int number = stoi(numStr);
                    list.push_back(number);
                    numStr = "";
                } else if (c == ']') {
                    int number = stoi(numStr);
                    list.push_back(number);
                    numStr = "";
                    listMode = false;
                    input.push_back(list);
                    list = {};
                } else {
                    numStr += c;
                }

            } else {
                if (c == ',' ) {
                    if (numStr != "") {
                        int number = stoi(numStr);
                        input.push_back(number);
                        numStr = "";
                    }
                } else if (c == '[') {
                    listMode = true;
                } else {
                    numStr += c;
                }
            }
        }
        if (numStr != "")
            input.push_back(stoi(numStr));

        return input;
    }

    experimental::optional<int> device_manager::get_cluster_ID(Input &input) {
        int clusterID = -1;
        bool IDgotten = false;
        auto result = eval(prog, input);
        if (!result) {
            cerr << "DSL program returned null" << endl;
        } else {
            Output o = result.value();
            experimental::optional<int> i = o.integer();
            experimental::optional<vector<int>> l = o.list();

            if (i) {
                clusterID = i.value();
                IDgotten = true;
            } else if (l) {
                vector<int> vals = l.value();
                clusterID = vals[0]; // Don't know if this is the best way
                IDgotten = true;
            }
        }

        if (IDgotten) {
            return experimental::optional<int>(clusterID);
        }
        return experimental::optional<int>();
        
    }


    void device_manager::print_device_clusterings() {
        for (pair<int, dev_cluster*> pair : DC_clusters) {
            cout << "DC_clusters[" << pair.first << "]" << endl;

            if (pair.second) 
                for (Input dev : *pair.second) {
                    cout << "\tdev:";
                    for (Value v : dev) 
                        cout << v << ',';
                     
                    cout << endl;
                }
            cout << endl;
        }
    }

    void device_manager::print_context_clusterings() {
        for (pair<string, vector< pair<int, dev_cluster*> >* > pair : context_clusters) {
            cout << "context_clusters[" << pair.first << "]" << endl;

            for (std::pair<int, dev_cluster*> cluster : *(pair.second)) {
                cout << "\tusing dev cluster " << cluster.first << endl;
                for (Input dev : *(cluster.second)) {
                    cout << "\t\tdev:";
                    for (Value v : dev) 
                        cout << v << ',';
                 
                    cout << endl;
                }
            }
            cout << endl;
        }
    }

    void device_manager::addDev(Input input) {
        experimental::optional<int> optClustID = get_cluster_ID(input);
        if (optClustID) {
            int clustID = optClustID.value();
            if (!DC_clusters[clustID])
                DC_clusters[clustID] = new dev_cluster();
            DC_clusters[clustID]->push_back(input);
        }
    }

    void device_manager::addCon(string context, int devClusterID) {
        vector<Input> *devCluster = DC_clusters[devClusterID];
        if (!devCluster) {
            return;
        }
        if (!context_clusters[context])
            context_clusters[context] = new vector<pair<int, dev_cluster*>>();
        context_clusters[context]->push_back(pair<int, dev_cluster*>(devClusterID, devCluster));
        DC_cluster_dependants[devClusterID].push_back(context);
    }

    void device_manager::rmCon(string context) {
        context_clusters.erase(context);
    }

    void device_manager::rmDev(Input input) {
        experimental::optional<int> optClustID = get_cluster_ID(input);
        if (optClustID) {
            int clustID = optClustID.value();
            dev_cluster *devClust = DC_clusters[clustID];

            if (devClust) {
                // Remove device from its device cluster
                for (auto i = devClust->begin(); i != devClust->end(); ++i) {
                    if (*i == input) {
                        devClust->erase(i);
                        break;
                    }
                }

                // Update context clusters that depends on the device cluster if it's removed
                if (devClust->empty()) {
                    for (string conClustName : DC_cluster_dependants[clustID]) {
                        if (context_clusters[conClustName] == nullptr) {
                            context_clusters.erase(conClustName); // Because accessing (above) generates an entry
                            continue;
                        }

                        vector< pair<int, dev_cluster*>> *conClust = context_clusters[conClustName];
                        for (auto i = conClust->begin(); i != conClust->end(); ++i)
                            if ((*i).first == clustID) {
                                conClust->erase(i);
                                break;
                            }
                        if (conClust->empty())
                            context_clusters.erase(conClustName);
                    }

                    DC_cluster_dependants[clustID].clear();
                    DC_clusters.erase(clustID);
                    delete devClust;
                }
            } else {
                DC_clusters.erase(clustID); // to remove entry implicitly created by calling DC_clusters[clustID]
            }
        }
    }

}
