#include <iostream>
#include <sstream>
#include <bitset>
#include <vector>
#include <algorithm>
#include <cmath>
#include <regex>
#include <numeric>

using namespace std;

struct Result {
    string ip_address;
    string netmask;
    string cidr;
    string wildcard;
    string ip_class;
    string network_address;
    string host_min;
    string host_max;
    string broadcast;
    string total_subnets;
    string total_hosts;
    string specific_info;
    string error_msg;
};

string classify(int ip) {
    if (ip < 128) return "Class A";
    if (ip < 192) return "Class B";
    if (ip < 224) return "Class C";
    if (ip < 240) return "Class D";
    if (ip < 256) return "Class E";
    return "";
}

int classifyCIDR(int cidr) {
    if (cidr < 16) return 0;
    if (cidr < 24) return 1;
    if (cidr < 32) return 2;
    return -1;
}

unsigned int unpackInt(int i) {
    return 0xffffffffU << (32 - i);
}

string intToQdot(int i) {
    stringstream ss;
    ss << ((i >> 24) & 0xFF) << "." << ((i >> 16) & 0xFF) << "." << ((i >> 8) & 0xFF) << "." << (i & 0xFF);
    return ss.str();
}

int qdotToInt(string ip) {
    vector<int> v;
    stringstream ss(ip);
    string s;
    while (getline(ss, s, '.')) {
        v.push_back(stoi(s));
    }
    return (v[0] << 24) | (v[1] << 16) | (v[2] << 8) | v[3];
}

int hostPerSubnet(int cidr) {
    return cidr >= 2 ? pow(2, (32 - cidr)) - 2 : cidr;
}

string usableHost(int host_total) {
    if (host_total < 2) return "0";
    stringstream ss;
    ss << fixed << host_total;
    string s = ss.str();
    int n = s.length();
    for (int i = n - 3; i > 0; i -= 3) {
        s.insert(i, ".");
    }
    return s;
}

string getNetworkAddress(int ip_bit, int netmask_bit) {
    return intToQdot(ip_bit & netmask_bit);
}

string getBroadcastAddress(int ip_bit, int netmask_bit) {
    return intToQdot(ip_bit | (~netmask_bit & 0xFFFFFFFF));
}

Result ipCalc(string ip, int cat, string val) {
    Result result;
    result.ip_address = ip;

    if (!regex_match(ip, regex("^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$"))) {
        result.error_msg = "Invalid IP Address!";
        return result;
    }

    int ip_int = ip.substr(0, ip.find_first_of('.')).empty() ? 0 : stoi(ip.substr(0, ip.find_first_of('.')));
    result.ip_class = classify(ip_int);
    if(ip_int == 127 && ip != "127.0.0.0") {
        result.specific_info = "Loopback Address";
    }
    if(ip_int == 10) {
        result.specific_info = "Private Address";
    }
    if(ip_int == 169) {
        ip_int = stoi(ip.substr(ip.find_first_of('.') + 1, ip.find_last_of('.') - ip.find_first_of('.') - 1));
        if(ip_int == 254) {
            result.specific_info = "Link Local Address";
        }
    }
    if(ip_int == 172) {
        ip_int = stoi(ip.substr(ip.find_first_of('.') + 1, ip.find_last_of('.') - ip.find_first_of('.') - 1));
        if(ip_int >= 16 && ip_int <= 31) {
            result.specific_info = "Private Address";
        }
    }
    if(ip_int == 192) {
        ip_int = stoi(ip.substr(ip.find_first_of('.') + 1, ip.find_last_of('.') - ip.find_first_of('.') - 1));
        if(ip_int == 168) {
            result.specific_info = "Private Address";
        }
    }

    if (cat == 0 && !regex_match(val, regex("^(((255\\.){3}(255|254|252|248|240|224|192|128|0+))|((255\\.){2}(255|254|252|248|240|224|192|128|0+)\\.0)|((255\\.)((255|254|252|248|240|224|192|128|0+)\\.){2})|((255|254|252|248|240|224|192|128|0+)(\\.0+){3}))$"))) {
        result.error_msg = "Invalid Netmask!";
        return result;
    }

    if (cat == 1 && !regex_match(val, regex("\\b([1-9]|[12][0-9]|3[0-2])\\b"))) {
        result.error_msg = "Invalid CIDR!";
        return result;
    }

    if (cat == 2 && !regex_match(val, regex("^[1-9]\\d*$"))) {
        result.error_msg = "Invalid Host number!";
        return result;
    }

    if (cat == 0) {
        result.netmask = val;
        result.cidr = val;
        vector<int> v;
        stringstream ss(val);
        string s;
        while (getline(ss, s, '.')) {
            v.push_back(stoi(s));
        }
        int cidr = 0;
        for (int i = 0; i < 4; i++) {
            cidr += bitset<8>(v[i]).count();
        }
        result.cidr = "/" + to_string(cidr);
        result.total_hosts = usableHost(hostPerSubnet(cidr));
    }

    if (cat == 1) {
        result.cidr = "/" + val;
        int cidr = stoi(val);
        result.netmask = intToQdot(unpackInt(cidr));
        result.total_hosts = usableHost(hostPerSubnet(cidr));
    }

    if (cat == 2) {
        int host_total = stoi(val);
        int cidr = 32 - ceil(log2(host_total + 2));
        result.cidr = "/" + to_string(cidr);
        result.netmask = intToQdot(unpackInt(cidr));
        result.total_hosts = usableHost(hostPerSubnet(cidr));
    }

    int cidr_class = classifyCIDR(stoi(result.cidr.substr(1)));
    if (result.ip_class == "Class A" && cidr_class != 0) {
        result.error_msg = "IP Address and Netmask/CIDR out of range!";
        return result;
    }
    if (result.ip_class == "Class B" && cidr_class != 1) {
        result.error_msg = "IP Address and Netmask/CIDR out of range!";
        return result;
    }
    if (result.ip_class == "Class C" && cidr_class != 2) {
        result.error_msg = "IP Address and Netmask/CIDR out of range!";
        return result;
    }

    vector<int> netmask_arr;
    stringstream ss(result.netmask);
    string s;
    while (getline(ss, s, '.')) {
        netmask_arr.push_back(stoi(s));
    }

    int ip_bit = qdotToInt(ip);
    int netmask_bit = qdotToInt(result.netmask);

    result.wildcard = intToQdot(~netmask_bit);
    result.network_address = getNetworkAddress(ip_bit, netmask_bit);
    result.broadcast = getBroadcastAddress(ip_bit, netmask_bit);

    vector<string> network_inc;
    stringstream ss1(result.network_address);
    while (getline(ss1, s, '.')) {
        network_inc.push_back(s);
    }
    vector<string> broadcast_dec;
    stringstream ss2(result.broadcast);
    while (getline(ss2, s, '.')) {
        broadcast_dec.push_back(s);
    }
    int n = network_inc.size();
    network_inc[n - 1] = to_string(stoi(network_inc[n - 1]) + 1);
    broadcast_dec[n - 1] = to_string(stoi(broadcast_dec[n - 1]) - 1);
    result.host_min = accumulate(begin(network_inc), end(network_inc), string(""), [](string a, string b) { return a + b + "."; });
    result.host_min.pop_back();
    result.host_max = accumulate(begin(broadcast_dec), end(broadcast_dec), string(""), [](string a, string b) { return a + b + "."; });
    result.host_max.pop_back();

    auto subnetBlockTotal = [&]() -> int {
        if (result.ip_class == "Class A") {
            string pow = bitset<8>(netmask_arr[1]).to_string() + bitset<8>(netmask_arr[2]).to_string() + bitset<8>(netmask_arr[3]).to_string();
            pow.erase(remove(begin(pow), end(pow), '0'), end(pow));
            return pow.empty() ? 1 : pow.size() == 1 ? 2 : pow.size() == 2 ? 4 : pow.size() == 3 ? 8 : pow.size() == 4 ? 16 : pow.size() == 5 ? 32 : pow.size() == 6 ? 64 : pow.size() == 7 ? 128 : pow.size() == 8 ? 256 : 0;
        }
        if (result.ip_class == "Class B") {
            string pow = bitset<8>(netmask_arr[2]).to_string() + bitset<8>(netmask_arr[3]).to_string();
            pow.erase(remove(begin(pow), end(pow), '0'), end(pow));
            return pow.empty() ? 1 : pow.size() == 1 ? 2 : pow.size() == 2 ? 4 : pow.size() == 3 ? 8 : pow.size() == 4 ? 16 : pow.size() == 5 ? 32 : pow.size() == 6 ? 64 : pow.size() == 7 ? 128 : pow.size() == 8 ? 256 : 0;
        }
        if (result.ip_class == "Class C") {
            string pow = bitset<8>(netmask_arr[3]).to_string();
            pow.erase(remove(begin(pow), end(pow), '0'), end(pow));
            return pow.empty() ? 1 : pow.size() == 1 ? 2 : pow.size() == 2 ? 4 : pow.size() == 3 ? 8 : pow.size() == 4 ? 16 : pow.size() == 5 ? 32 : pow.size() == 6 ? 64 : pow.size() == 7 ? 128 : pow.size() == 8 ? 256 : 0;
        }
        return 0;
    };

    result.total_subnets = to_string(subnetBlockTotal());

    return result;
}

int main (int argc, char *argv[]) {
    string ip, val;
    int cat;
    
    if (argc == 3) {
        ip = argv[1];
        val = argv[2];
        cat = 0;
    } else if (argc == 4) {
        ip = argv[1];
        val = argv[2];
        cat = stoi(argv[3]);
    } 
#if 0
    else {
        cout << "Enter IP Address: ";
        cin >> ip;
        cout << "Enter Netmask/CIDR/Host: ";
        cin >> val;
        cout << "Enter Category (0 = Netmask, 1 = CIDR, 2 = Host): ";
        cin >> cat;
    }
#endif

    Result result = ipCalc(ip, cat, val);
    if (result.error_msg.empty()) {
        cout << "IP Address: " << result.ip_address << endl;
        cout << "Netmask: " << result.netmask << endl;
        cout << "CIDR: " << result.cidr << endl;
        cout << "Wildcard: " << result.wildcard << endl;
        cout << "IP Class: " << result.ip_class << endl;
        cout << "Network Address: " << result.network_address << endl;
        cout << "Host Min: " << result.host_min << endl;
        cout << "Host Max: " << result.host_max << endl;
        cout << "Broadcast Address: " << result.broadcast << endl;
        cout << "Total Subnets: " << result.total_subnets << endl;
        cout << "Total Hosts: " << result.total_hosts << endl;
        if (!result.specific_info.empty()) {
            cout << "Specific Info: " << result.specific_info << endl;
        }
    } else {
        cout << result.error_msg << endl;
    }
    return 0;
}
