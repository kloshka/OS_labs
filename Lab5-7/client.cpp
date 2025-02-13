#include "node.h"
#include "net_func.h"
#include "set"
#include <signal.h>
#include <chrono>
#include <thread>
#include <mutex>
#include <atomic>
#include <map>

static std::mutex nodes_mutex;

int main() {
    std::set<int> all_nodes;
    std::string prog_path = "./worker";
    Node me(-1);
    all_nodes.insert(-1);
    
    std::string command;
    while (std::cin >> command) {
        if (command == "create") {
            int id_child, id_parent;
            std::cin >> id_child >> id_parent;
            if (all_nodes.find(id_child) != all_nodes.end()) {
                std::cout << "Error: Already exists" << std::endl;
            } else if (all_nodes.find(id_parent) == all_nodes.end()) {
                std::cout << "Error: Parent not found" << std::endl;
            } else if (id_parent == me.id) {
                std::string ans = me.Create_child(id_child, prog_path);
                std::cout << ans << std::endl;
                all_nodes.insert(id_child);
            } else {
                std::string str = "create " + std::to_string(id_child);
                std::string ans = me.Send(str, id_parent);
                std::cout << ans << std::endl;
                all_nodes.insert(id_child);
            }
        } else if (command == "ping") {
            int id_child;
            std::cin >> id_child;
            if (all_nodes.find(id_child) == all_nodes.end()) {
                std::cout << "Error: Not found" << std::endl;
            } else if (me.children.find(id_child) != me.children.end()) {
                std::string ans = me.Ping_child(id_child);
                std::cout << ans << std::endl;
            } else {
                std::string str = "ping " + std::to_string(id_child);
                std::string ans = me.Send(str, id_child);
                if (ans == "Error: not find") {
                    ans = "Ok: 0";
                }
                std::cout << ans << std::endl;
            }
        } else if (command == "exec") {
            int id;
            std::string cmd;
            std::cin >> id >> cmd;
            std::string msg = "exec " + cmd;
        
            if (all_nodes.find(id) == all_nodes.end()) {
                std::cout << "Error: Not found" << std::endl;
            } else {
                std::string ping_result = me.Send("ping " + std::to_string(id), id);
                if (ping_result == "Ok: 0" || ping_result == "Error: not find") {
                    std::cout << "Error: Node is unavailable" << std::endl;
                    all_nodes.erase(id);
                } else {
                    std::string ans = me.Send(msg, id);
                    std::cout << ans << std::endl;
                }
            }
        } else if (command == "remove") {
            int id;
            std::cin >> id;
            std::string msg = "remove";
            if (all_nodes.find(id) == all_nodes.end()) {
                std::cout << "Error: Not found" << std::endl;
            } else {
                std::string ans = me.Send(msg, id);
                if (ans != "Error: not find") {
                    std::istringstream ids(ans);
                    int tmp;
                    while (ids >> tmp) {
                        all_nodes.erase(tmp);
                    }
                    ans = "Ok";
                    if (me.children.find(id) != me.children.end()) {
                        my_net::unbind(me.children[id], me.children_port[id]);
                        me.children[id]->close();
                        me.children.erase(id);
                        me.children_port.erase(id);
                    }
                }
                std::cout << ans << std::endl;
            }
        }
    }

    me.Remove();
    return 0;
}