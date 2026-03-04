#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <algorithm>

using namespace std;



struct Record {
    std::string date;
    std::string ticker;
    double open = 0.0;
    double high = 0.0;
    double low  = 0.0;
    double close = 0.0;
    long long volume = 0;
    double dividends = 0.0;
    double splits = 0.0;

    bool operator<(const Record& other) const {
    return close < other.close;
}
};

class StockDB {
private:
    std::vector<Record> data;

    std::unordered_map<std::string, std::vector<Record>> byDate;

    std::unordered_map<std::string, std::vector<Record>> byTicker; // nije nužno, ali može ubrzati neke upite
    

    std::unordered_map<std::string, 
                                    std::unordered_map<std::string, Record>> byTickerDate; // key = "TICKER|DATE"
    


    // jednostavan split po zarezu (radi ako CSV nema navodnike i zareze unutar polja)
    static std::vector<std::string> split(const std::string& line) {
        std::vector<std::string> parts;
        std::string cur;
        cur.reserve(line.size());

        for (char c : line) {
            if (c == ',') {
                parts.push_back(cur);
                cur.clear();
            } else {
                cur.push_back(c);
            }
        }
        parts.push_back(cur);
        return parts;
    }

public:
    bool loadFromCSV(const std::string& filename, int maxLines = -1) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "Ne mogu otvoriti CSV: " << filename << "\n";
            return false;
        }

        std::string line;

        // preskoči header
        std::getline(file, line);

        int count = 0;

        while (std::getline(file, line)) {
            if (line.empty()) continue;
            if (maxLines != -1 && count >= maxLines) break;

            auto cols = split(line);
            if (cols.size() != 9) continue;

            Record r;
            
            try {
                r.date = cols[0];
                r.ticker = cols[1];
                r.open = std::stod(cols[2]);
                r.high = std::stod(cols[3]);
                r.low  = std::stod(cols[4]);
                r.close= std::stod(cols[5]);
                r.volume = std::stoll(cols[6]);
                r.dividends = std::stod(cols[7]);
                r.splits = std::stod(cols[8]);
            } catch (...) {
                continue; // preskoči loš red
            }

            data.push_back(r);
            byDate[r.date].push_back(r);
            byTicker[r.ticker].push_back(r);
            byTickerDate[r.ticker][r.date] = r;

            count++;
        }

        std::cout << "Ucitao redaka: " << count << "\n";
        std::cout << "byTickerDate size: " << byTickerDate.size() << "\n";
        return true;
    }
    
    
    // 1) Dohvati sve podatke za određeni datum
    std::vector<Record> PoDatumu(const std::string& datum) const {
            auto it = byDate.find(datum);
            if (it == byDate.end()) return {};
            return it->second; // kopija (ok za projekt)
    }

    

    // 2) Prosječna završna cijena (Close) za ticker kroz cijeli skup
    double ProsjekCijeneDionice(const std::string& ticker) const {
        auto it = byTicker.find(ticker);
        if (it == byTicker.end()) {
            std::cout << "Nema podataka za taj ticker\n";
            return 0.0;
        }
        double suma = 0.0;

        for (const auto& record : it->second) {
            suma += record.close;
        }

        return suma / it->second.size();


    }

    // 3) Najviši High u rasponu datuma za ticker
    double NajvisaCijenaDioniceIzmedjuDatuma(const string& ticker,
                                          const string& startDate,
                                          const string& endDate) {

    auto it = byTicker.find(ticker);
    if(it == byTicker.end()) return 0.0;

    double maxPrice = -1.0;
    bool found = false;

    for(const auto& record : it->second) {

        if(record.date >= startDate && record.date <= endDate) {

            if(!found || record.high > maxPrice) {
                maxPrice = record.high;
                found = true;
            }
        }
    }

    return found ? maxPrice : 0.0;
}

    vector<string> SviTickeri() const {
        vector<string> tickers;
        for (const auto& par : byTicker) {
            tickers.push_back(par.first);
        }
        return tickers;
    }

    bool PostojiTicker(const string& ticker) const {
        return byTicker.find(ticker) != byTicker.end();
        
    }

    int KolikoDatumaDionicaIznadZavrsne(const string& ticker) {
    auto it = byTicker.find(ticker);
    if (it == byTicker.end()) return 0;

    int count = 0;
    for (const auto& record : it->second) {
        if (record.close > record.open) {
            count++;
        }
    }
    return count;
}

    void printFirst(int n = 5) const {
        for (int i = 0; i < n && i < (int)data.size(); i++) {
            std::cout << data[i].date << " "
                      << data[i].ticker << " "
                      << data[i].close << "\n";
        }
    }

    double CijenaDioniceNaDatum(const string& ticker, const string& datum) {
        auto it = byTicker.find(ticker);
        if (it == byTicker.end()) return 0.0;

        for (const auto& record : it->second) {
            if (record.date == datum) {
                return record.close;
            }
        }
        return 0.0; // nije nađen taj datum
    }

    string SviDatumiIZavrsneCijeneDionice(const string& ticker) {
        auto it = byTicker.find(ticker);
        if (it == byTicker.end()) return "";

        string result;
        for (const auto& record : it->second) {
            result += record.date + ": " + to_string(record.close) + "\n";
        }
        return result;
    }

    long double VolumenDionice(const string& ticker) {
        auto it = byTicker.find(ticker);
        if (it == byTicker.end()) return 0.0;

        long double totalVolume = 0.0;
        for (const auto& record : it->second) {
            totalVolume += record.volume;
        }
        return totalVolume;
    }

    bool PostojiDatumIDionica(const string& ticker, const string& datum) {
        auto it = byTicker.find(ticker);
        if (it == byTicker.end()) return false;

        for (const auto& record : it->second) {
            if (record.date == datum) {
                return true;
            }
        }
        return false;
    }

    string OtvaranjeIZatvaranjeDioniceIDatuma(const string& ticker, const string& datum) {

    auto itTicker = byTickerDate.find(ticker);
    if (itTicker == byTickerDate.end())
        return "Ticker ne postoji.";

    auto itDate = itTicker->second.find(datum);
    if (itDate == itTicker->second.end())
        return "Ne postoji zapis za taj datum.";

    const Record& r = itDate->second;

    return datum + " " + ticker +
           " open=" + to_string(r.open) +
           " close=" + to_string(r.close);
}

    double dividenteDionicePoDatumu(const string& ticker, const string& datum) {
    auto itTicker = byTickerDate.find(ticker);
    if (itTicker == byTickerDate.end())
        return 0.0;
    
    auto itDate = itTicker->second.find(datum);
    if (itDate == itTicker->second.end())
        return 0.0;

    return itDate->second.dividends;
}


    vector<Record> Top10VolumenNaDatum(const string& datum) const {
    auto it = byDate.find(datum);
    if (it == byDate.end()) return {};

    vector<Record> v = it->second;   // kopija zapisa za taj datum

    sort(v.begin(), v.end(), [](Record a, Record b) {
        return a.volume > b.volume;  // najveći volumen prvi
    });

    if (v.size() > 10)
        v.resize(10);

    return v;
}


    vector<Record> Najnizih5ZavrsnihCijena() const {
    vector<Record> v = data;

    sort(v.begin(), v.end());  // koristi operator<

    if (v.size() > 5) v.resize(5);

    return v;
}
        
void UnesiRecord(const Record& r) {
    data.push_back(r);
    byDate[r.date].push_back(r);
    byTicker[r.ticker].push_back(r);

    // ako koristiš byTickerDate (ticker -> (datum -> Record))
    byTickerDate[r.ticker][r.date] = r;
}

bool EraseRecord(const string& ticker, const string& datum) {

    bool obrisano = false;

    // data
    for (int i = 0; i < data.size(); i++) {
        if (data[i].ticker == ticker && data[i].date == datum) {
            data.erase(data.begin() + i);
            obrisano = true;
            break;
        }
    }

    //  byDate
    if (byDate.count(datum)) {
        auto& v = byDate[datum];

        for (int i = 0; i < v.size(); i++) {
            if (v[i].ticker == ticker) {
                v.erase(v.begin() + i);
                break;
            }
        }

        if (v.empty())
            byDate.erase(datum);
    }

    // byTicker
    if (byTicker.count(ticker)) {
        auto& v = byTicker[ticker];

        for (int i = 0; i < v.size(); i++) {
            if (v[i].date == datum) {
                v.erase(v.begin() + i);
                break;
            }
        }

        if (v.empty())
            byTicker.erase(ticker);
    }

    // byTickerDate
    if (byTickerDate.count(ticker)) {
        byTickerDate[ticker].erase(datum);

        if (byTickerDate[ticker].empty())
            byTickerDate.erase(ticker);
    }

    return obrisano;
}

    
};
