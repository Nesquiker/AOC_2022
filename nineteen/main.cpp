#include <iostream>
#include <fstream>
#include <vector>
#include <deque>

struct blueprint {
    int id = 0;
    std::vector<int> ore_cost {0, 0, 0};
    std::vector<int> clay_cost {0, 0, 0};
    std::vector<int> obsidian_cost {0, 0, 0};
    std::vector<int> geode_cost {0, 0, 0};
};

struct branch {
    std::vector<int> resources {0, 0, 0, 0};
    std::vector<int> robots  {1, 0, 0, 0};
    int turn = 0;
};

std::vector<blueprint> readBlueprints (const std::string& file_path) {
    std::ifstream data(file_path);
    std::vector<blueprint> blueprints;
    for (std::string line; std::getline(data, line);) {
        std::string buffer;
        std::vector<int> values;
        for (char c : line) {
            if (isdigit(c)) {
                buffer.push_back(c);
            } else if (!buffer.empty()) {
                values.emplace_back(std::stoi(buffer));
                buffer = "";
            }
        }
        blueprint current;
        current.id = values[0];
        current.ore_cost[0] = values[1];
        current.clay_cost[0] = values[2];
        current.obsidian_cost[0] = values[3];
        current.obsidian_cost[1] = values[4];
        current.geode_cost[0] = values[5];
        current.geode_cost[2] = values[6];
        blueprints.emplace_back(current);
    }
    return blueprints;
}

bool testEfficiency (const branch& testee, const std::vector<int>& max_costs) {
    const int max_ore = max_costs[0] * 5;
    const int max_clay = max_costs[1] * 10;
    const int max_obs = max_costs[2] * 4;
    return (testee.resources[0] < max_ore) && (testee.resources[1] < max_clay) && (testee.resources[2] < max_obs);
}

std::vector<std::pair<bool, std::vector<int>>> calcFactories (const blueprint& bp, const std::vector<int>& resources) {
    std::vector<std::pair<bool, std::vector<int>>> output;
    std::pair<bool, std::vector<int>> can_buy_and_rem {true, resources};
    output.emplace_back(can_buy_and_rem);
    for (int i = 0 ; i < 4; i++) {
        std::vector<int> r_cost;
        std::vector<int> local_res = resources;
        if (i == 0) {
            r_cost = bp.ore_cost;
        } else if (i == 1) {
            r_cost = bp.clay_cost;
        } else if (i == 2) {
            r_cost = bp.obsidian_cost;
        } else if (i == 3) {
            r_cost = bp.geode_cost;
        }
        bool can_afford = true;
        for (int j = 0; j < 3; j++) {
            local_res[j] -= r_cost[j];
            if (local_res[j] < 0) {
                can_afford = false;
                break;
            }
        }
        if (can_afford) {
            can_buy_and_rem = {true, local_res};
            output.emplace_back(can_buy_and_rem);
        } else {
            can_buy_and_rem = {false, local_res};
            output.emplace_back(can_buy_and_rem);
        }
    }
    return output;
}


bool canBeGreatest (const branch& pos, const blueprint& bp, const int max_val) {
    int rem_turns = 32 - pos.turn;
    int diff = max_val - pos.resources.back() - pos.robots.back() * (rem_turns + 1);
    int additional_geodes = 0;
    int rem_count = rem_turns;
    while (diff >= 0) {
        additional_geodes++;
        diff -= rem_count;
        rem_count--;
        if (rem_count < 0) {
            return false;
        }
    }
    int obsidian = pos.resources[2] + pos.robots[2] * (rem_turns) - additional_geodes * bp.geode_cost[2];
    int additional_obsidians = 0;
    rem_count = rem_turns;
    while (obsidian < 0) {
        additional_obsidians++;
        obsidian += rem_count;
        rem_count--;
        if (rem_count < 0) {
            return false;
        }
    }
    int clay = pos.resources[1] + pos.robots[1] * (rem_turns) - additional_obsidians * bp.obsidian_cost[1];
    int additional_clay = 0;
    rem_count = rem_turns;
    while (clay < 0) {
        additional_clay++;
        clay += rem_count;
        rem_count--;
        if (rem_count < 0) {
            return false;
        }
    }
    int ore = pos.resources[0] + pos.robots[0] * (rem_turns) - additional_obsidians * bp.obsidian_cost[0] -
            additional_geodes * bp.geode_cost[0] - additional_clay * bp.clay_cost[0];
    int additional_ore = 0;
    rem_count = rem_turns;
    while (ore < 0) {
        additional_ore++;
        ore += rem_count - bp.ore_cost[0];
        rem_count--;
        if (rem_count < 0) {
            return false;
        }
    }
    const int total_building_turns = additional_ore + additional_geodes + additional_obsidians + additional_clay;
    if (total_building_turns <= rem_turns) {
        return true;
    } else {
        return false;
    }
}

int findMaxGeodes (const blueprint& bp) {
    int minute = 0;
    const int max_time = 32;
    std::vector<int> r_cost;
    std::vector<int> max_costs (3);
    for (int i = 0; i < 4; i++) {
        if (i == 0) {
            r_cost = bp.ore_cost;
        } else if (i == 1) {
            r_cost = bp.clay_cost;
        } else if (i == 2) {
            r_cost = bp.obsidian_cost;
        } else if (i == 3) {
            r_cost = bp.geode_cost;
        }
        for (int j = 0; j < 3; j++) {
            max_costs[j] = std::max(max_costs[j], r_cost[j]);
        }
    }

    std::vector<branch> bfs;
    bfs.emplace_back();
    int running_max = 0;
    while (!bfs.empty()) {
        auto current = bfs.back();
        bfs.pop_back();
        current.turn++;
        minute = current.turn;
        if (minute > max_time || !canBeGreatest(current, bp, running_max)) {
            running_max = std::max(running_max, current.resources.back());
            continue;
        }
        auto paths = calcFactories(bp, current.resources);
        for (int i = 0 ; i < paths.size(); i++) {
            if (!paths[i].first || (paths.back().first && i != paths.size() - 1)) {
                continue;
            }
            branch temp = current;
            temp.resources = paths[i].second;

            for (int j = 0; j < 4; j++) {
                temp.resources[j] += temp.robots[j];
            }
            if (!testEfficiency(temp, max_costs)) {
                continue;
            }
            if (i == 0) {
                if (temp.robots[1] == 0 && paths[1].first && paths[2].first) {
                    continue;
                } else if (temp.robots[2] == 0 && paths[1].first && paths[2].first && paths[3].first) {
                    continue;
                }
            } else {
                temp.robots[i - 1]++;
            }
            bfs.emplace_back(temp);
        }
    }
    return running_max;
}

int part_1 (const std::vector<blueprint>& blueprints) {
    std::vector<int> qualities;
    for (auto& blueprint : blueprints) {
        int geodes = findMaxGeodes(blueprint);
        const int quality = blueprint.id * geodes;
        qualities.emplace_back(quality);
    }
    int output = 0;
    for (int q : qualities) {
        output += q;
    }
    return output;
}

int part_2 (const std::vector<blueprint>& blueprints) {
    std::vector<int> geode_maxes;
    int count = 1;
    for (auto& blueprint : blueprints) {
        if (count > 3) {
            break;
        }
        int geodes = findMaxGeodes(blueprint);
        geode_maxes.emplace_back(geodes);
        count++;
    }
    int output = 1;
    for (int geode : geode_maxes) {
        output *= geode;
    }
    return output;
}

int main () {
    const std::string test_path = "../test.txt";
    const std::string real_path = "../input.txt";
    const auto test_blueprints = readBlueprints(test_path);
    const auto real_blueprints = readBlueprints(real_path);
//    const int test_ans_part_1 = part_1(test_blueprints);
//    const int real_ans_part_1 = part_1(real_blueprints);
//    std::cout << test_ans_part_1 << "\n";
//    std::cout << real_ans_part_1 << "\n";
    const int test_ans_part_2 = part_2(test_blueprints);
    const int real_ans_part_2 = part_2(real_blueprints);
    std::cout << test_ans_part_2 << "\n";
    std::cout << real_ans_part_2 << "\n";



    return 0;

}