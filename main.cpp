#include "StockDB.h"

using namespace std;
int main() {

    StockDB db;

    

    
    db.loadFromCSV("C:/Users/Admin/Documents/Algoritmi/archive/data.csv", 100000);

    db.printFirst();

    auto list = db.PoDatumu("1962-01-04");

    std::cout << "Pronadeno: " << list.size() << "\n";

    for(int i=0;i<5 && i<list.size();i++) {
    std::cout << list[i].ticker << " "
              << list[i].close << "\n";
    }

    cout << "Prosjek cijene dionice GD: " << db.ProsjekCijeneDionice("GD") << "\n";

    cout << "Najvisa cijena dionice GD izmedju 1962-01-01 i 1962-12-31: " << db.NajvisaCijenaDioniceIzmedjuDatuma("GD", "1962-01-01", "1962-12-31") << "\n";

    for (int i = 0; i < 10; i++) {
        cout << db.SviTickeri()[i] << "\n";
    }

    if (db.PostojiTicker("GD")) {
        cout << "GD postoji\n";
    } else {
        cout << "GD ne postoji\n";
    }

    cout << "Broj datuma kada je GD iznad zavrsne cijene: " << db.KolikoDatumaDionicaIznadZavrsne("GD") << "\n";

    double cijena = db.CijenaDioniceNaDatum("GD", "1962-01-04");
    cout << "Cijena dionice GD na datum 1962-01-04: " << cijena << "\n";

    string datumi = db.SviDatumiIZavrsneCijeneDionice("GD");

    cout << "Svi datumi i zavrsne cijene dionice GD:\n" << datumi;

    db.PostojiDatumIDionica("GD", "1962-01-04") ? cout << "Postoji\n" : cout << "Ne postoji\n";
    

     string a = db.OtvaranjeIZatvaranjeDioniceIDatuma("GD", "1962-01-16");
        cout << a << endl;


        double div = db.dividenteDionicePoDatumu("CAT", "1962-01-16");
        cout << "Dividende dionice CAT na datum 1962-01-16: " << div << "\n";

        auto top10 = db.Top10VolumenNaDatum("1962-01-16");
        cout << "Top 10 dionica po volumenu na datum 1962-01-16:\n";
        for (const auto& r : top10) {
            cout << r.ticker << " " << r.volume << "\n";
        }

        vector<Record> rezultat = db.Najnizih5ZavrsnihCijena();

        for (const auto& r : rezultat) {
        cout << r.ticker << " "
             << r.date << " "
             << r.close << endl;
        }
}
