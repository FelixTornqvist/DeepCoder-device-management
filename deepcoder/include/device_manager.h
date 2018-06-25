#include "dsl/interpreter.h"
#include <vector>
#include <experimental/optional>

namespace dm {
    class device_manager {
        public:
            device_manager(std::string programFile);
            ~device_manager();

            dsl::Input getInput(std::string line);

            void addDev(dsl::Input input);
            void addCon(std::string context, int clusterID);
            void rmDev(dsl::Input input);
            void rmCon(std::string context);

            void print_context_clusterings();
            void print_device_clusterings();


        private:
            dsl::Program prog;

            using dev_cluster = std::vector<dsl::Input>;
            std::unordered_map<int, dev_cluster*> DC_clusters;
            std::unordered_map<std::string, std::vector< std::pair<int, dev_cluster*> >* > context_clusters;
            std::unordered_map<int, std::vector<std::string>> DC_cluster_dependants;

            std::experimental::optional<int> get_cluster_ID(dsl::Input &input);
    };
}
