#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <deque>
#include <map>
#include <unordered_set>
#include <set>
#include <map>
#include <bitset>


struct valve_node {
    std::string name;
    int flow = 0;
    std::vector<valve_node*> connected_nodes;
    int best_vals [30]{0};
    std::map<int, std::pair<int, valve_node*>> sig_nodes;
};

std::vector<valve_node*> createValves (const std::string& path) {
    std::ifstream data(path);
    std::map<std::string, valve_node*> node_name_to_object;
    std::vector<valve_node*> nodes;
    for (std::string line; std::getline(data, line);) {
        std::vector<std::string> node_list;
        int flow = 0;
        std::string buffer;
        for (int i = 5; i < line.size(); i++) {
            const char c = line[i];
            if (isupper(c) || std::isdigit(c)) {
                buffer.push_back(c);
            } else if (!buffer.empty()) {
                if (isdigit(buffer[0])) {
                    flow = std::stoi(buffer);
                } else {
                    node_list.push_back(buffer);
                }
                buffer = "";
            }
        }
        node_list.push_back(buffer);
        valve_node* node;
        if (node_name_to_object.contains(node_list.front())) {
            node = node_name_to_object[node_list.front()];
        } else {
            node = new valve_node;
            node -> name = node_list.front();
            node_name_to_object[node -> name] = node;
        }
        node -> flow = flow;
        node_list.erase(node_list.begin());
        for (auto& s : node_list) {
            valve_node* c_node;
            if (node_name_to_object.contains(s)) {
                c_node = node_name_to_object[s];
            } else {
                c_node = new valve_node;
                c_node -> name = s;
                node_name_to_object[s] = c_node;
            }
            node -> connected_nodes.emplace_back(c_node);
        }
        nodes.emplace_back(node);
    }
    return nodes;
}


std::map<int, std::pair<int, valve_node*>> findSigNodes (valve_node* valve) {
    std::unordered_set<valve_node*> visited;
    visited.insert(valve);
    std::map<int, std::pair<int, valve_node*>> output;
    std::deque<std::pair<valve_node*, int>> bfs_deque;
    bfs_deque.emplace_back(valve, 0);
    while (!bfs_deque.empty()) {
        std::pair<valve_node*, int> pos = bfs_deque.back();
        bfs_deque.pop_back();
        for (auto* node : pos.first -> connected_nodes) {
            if (visited.contains(node)) {
                continue;
            }
            visited.insert(node);
            if (node -> flow > 0) {
                output[node -> flow] = {pos.second + 1, node};
            }
            bfs_deque.emplace_front(node, pos.second + 1);
        }
    }
    return output;
}


void fillSigNodes (const std::vector<valve_node*>& valves) {
    for (auto* valve : valves) {
        if (valve -> flow > 0 || valve -> name == "AA") {
            valve -> sig_nodes = findSigNodes(valve);
        }
    }
}

std::set<int> findFlows (const std::vector<valve_node*>& valves) {
    std::set<int> flows;
    for (auto* valve : valves) {
        if (valve -> flow > 0) {
            flows.insert(valve -> flow);
        }
    }
    return flows;
}


struct possibility {
    int final_val = 0;
    int turn = 1;
    valve_node* node{};
    std::set<valve_node*> visited;
    std::bitset<30> v_bits;
};

int part_2 (const std::vector<std::pair<int, std::bitset<30>>>& records) {
    const int n = records.size();
    int running_max = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            const int one = records[i].first;
            const int two = records[j].first;
            const int combined = one + two;
            if (combined > running_max) {
                std::bitset<30> res = records[i].second & records[j].second;
                if (res.none()) {
                    running_max = combined;
                }
            }
        }
    }
    std::cout << records.size() << "\n";
    std::cout << running_max << "\n";
    return running_max;
}

int part_1 (const std::vector<valve_node*>& nodes) {
    // travel cost 1 minute: open cost 1 minute:
    const std::set<int> flows = findFlows(nodes);
    fillSigNodes(nodes);
    int running_max = 0;
    const int total_turns = 26;
    possibility current;
    for (auto* valve: nodes) {
        if (valve -> name == "AA") {
            current.node = valve;
            current.visited.insert(valve);
        }
    }
    std::vector<possibility> dfs_search {current};
    std::vector<std::pair<int, std::bitset<30>>> records;
    while (!dfs_search.empty()) {
        auto pos = dfs_search.back();
        dfs_search.pop_back();
        auto sig_nodes = pos.node -> sig_nodes;
        bool any_moves = false;
        for (auto n : sig_nodes) {
            if (pos.visited.contains(n.second.second)) {
                continue;
            }
            auto n_pos = pos;
            n_pos.visited.insert(n.second.second);
            const int flow = n.first;
            const int dist = n.second.first;
            const int turn_to_take = n_pos.turn + dist;
            if (turn_to_take >= 30) {
                continue;
            }
            const int final = n_pos.final_val + flow * (total_turns - turn_to_take);
            n.second.second -> best_vals[turn_to_take - 1] = final;
            n_pos.final_val = final;
            n_pos.node = n.second.second;
            n_pos.turn = turn_to_take + 1;
            n_pos.v_bits[flow] = true;
            dfs_search.emplace_back(n_pos);
            any_moves = true;
        }
        if (!any_moves || pos.turn > 16) {
            running_max = std::max(running_max, pos.final_val);
            records.emplace_back(pos.final_val, pos.v_bits);
        }
    }
    part_2 (records);
    return running_max;
}


int main() {
    const std::string path = "../input.txt";
    auto nodes = createValves(path);
    std::cout << part_1(nodes);
}