#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <vector> 
#include <limits> 
using namespace std;

// ===================== STRUCT ===================== 
// Struct untuk menyimpan data karakter
struct Karakter {
    string nama;
    int health;
    int defense;
    int attack;
};

struct Enemy {
    string nama;
    int health;
    int defense;
    int attack;
};

// Struct untuk menyimpan data user
struct User {
    string username;
    string password;
    bool is_admin;
};

struct UserCharacter {
    string nama;
    int health;
    int defense;
    int attack;
};

struct UserData {
    string username;
    string password; 
    int coins;
    vector<UserCharacter> characters;
};

// Const
const int maks_karakter = 5;
const int maks_musuh = 5;
const int maks_user = 10;
const int maks_attempt = 3;

// Global variables
Karakter karakter[maks_karakter];
Enemy musuh[maks_musuh];
User users[maks_user];
int jumlahKarakter = 0;
int jumlahMusuh = 0;
int jumlahUser = 0;
bool is_login = false;
bool is_admin = false;
string userSekarang;
enum SearchType { ATTACK, HEALTH, DEFENSE };
UserData dataUserSekarang;
vector<UserData> dataUser;

void bersihkanBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void enter(bool tampilkan_enter = true) {
    cin.clear();

    if (tampilkan_enter) {
        cout << "\nTekan Enter untuk kembali ke menu...";
    }
    cin.ignore(1000, '\n');
}

// ===================== CSV FUNCTION ===================== 
// Fungsi untuk menyimpan data ke CSV
void simpanKeCSV() {
    ofstream file("csvFile/charactersData.csv");
    try {
        // Cek apakah file bisa dibuka
        if (!file.is_open()) {
            throw runtime_error("Gagal membuka file characters.csv untuk ditulis!");
        }

        // Header CSV
        file << "Nama,Attack,Health,Defense\n";

        // Tulis data
        for (int i = 0; i < jumlahKarakter; i++) {
            file << karakter[i].nama << ","
                 << karakter[i].attack << ","
                 << karakter[i].health << ","
                 << karakter[i].defense << "\n";
        }

        // Cek apakah penulisan berhasil
        if (file.fail()) {
            throw runtime_error("Gagal menulis data ke file!");
        }

    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
    }
    file.close(); 
}

void simpanMusuhKeCSV() {
    ofstream file("csvFile/enemy.csv");
    try {
        if (!file.is_open()) {
            throw runtime_error("Gagal membuka file enemy.csv untuk ditulis!");
        }

        file << "Nama,Attack,Health,Defense\n";
        
        for (int i = 0; i < jumlahMusuh; i++) {
            file << musuh[i].nama << ","
                 << musuh[i].attack << ","
                 << musuh[i].health << ","
                 << musuh[i].defense << "\n";

            if (file.fail()) {
                throw runtime_error("Gagal menulis data musuh ke file!");
            }
        }
        
    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
    }
    if (file.is_open()) {
        file.close();
    }
}

void muatKarakterDariCSV() {
    ifstream file("csvFile/charactersData.csv");
    try {
        // Cek apakah file ada
        if (!file.is_open()) {
            throw runtime_error("File characters.csv tidak ditemukan!");
        }

        string line;
        getline(file, line); // Skip header

        // Reset jumlahKarakter sebelum memuat ulang
        jumlahKarakter = 0;

        while (getline(file, line)) {
            if (line.empty()) continue; // Skip baris kosong

            vector<string> row;
            stringstream ss(line);
            string cell;

            // Split baris CSV
            while (getline(ss, cell, ',')) {
                row.push_back(cell);
            }

            // Validasi format CSV (harus 5 kolom)
            if (row.size() != 4) {
                throw runtime_error("Format file CSV tidak valid (kolom tidak sesuai)!");
            }

            // Cek kapasitas array
            if (jumlahKarakter >= maks_karakter) {
                throw runtime_error("Kapasitas karakter penuh, tidak bisa memuat lebih banyak data.");
            }

            // Simpan data ke struct
            karakter[jumlahKarakter].nama = row[0];
            
            try {
                karakter[jumlahKarakter].attack = stoi(row[1]);
                karakter[jumlahKarakter].health = stoi(row[2]);
                karakter[jumlahKarakter].defense = stoi(row[3]);
            } catch (const invalid_argument& e) {
                throw runtime_error("Data attack/health/defense harus angka!");
            } catch (const out_of_range& e) {
                throw runtime_error("Data attack/health/defense terlalu besar!");
            }

            jumlahKarakter++;
        }

        cout << "Data karakter berhasil dimuat (" << jumlahKarakter << " entri)\n";
    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
        jumlahKarakter = 0; // Reset data jika error
    }
    file.close();
}

void muatMusuhDariCSV() {
    ifstream file("csvFile/enemy.csv");
    try {
        if (!file.is_open()) {
            throw runtime_error("File enemy.csv tidak ditemukan!");
        }

        string line;
        getline(file, line); // Skip header
        jumlahMusuh = 0;

        while (getline(file, line)) {
            if (line.empty()) continue;

            vector<string> row;
            stringstream ss(line);
            string cell;

            while (getline(ss, cell, ',')) {
                row.push_back(cell);
            }

            if (row.size() != 4) {
                throw runtime_error("Format file enemy.csv tidak valid!");
            }

            if (jumlahMusuh >= maks_musuh) {
                throw runtime_error("Kapasitas musuh penuh!");
            }

            musuh[jumlahMusuh].nama = row[0];
            
            try {
                musuh[jumlahMusuh].attack = stoi(row[1]);
                musuh[jumlahMusuh].health = stoi(row[2]);
                musuh[jumlahMusuh].defense = stoi(row[3]);
            } catch (...) {
                throw runtime_error("Data stat musuh harus angka!");
            }

            jumlahMusuh++;
        }

        cout << "Data musuh berhasil dimuat (" << jumlahMusuh << " entri)\n";
    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
        jumlahMusuh = 0;
    }
    file.close();
}

void simpanUserData() {
    ofstream file("csvFile/userData.csv");
    try {
        if (!file.is_open()) {
            throw runtime_error("Gagal membuka file userData.csv untuk ditulis!");
        }

        file << "Username,Password,Coins,Characters\n";
        
        for (const auto& user : dataUser) {
            file << user.username << "," 
                 << user.password << "," 
                 << user.coins << ",";
                 
            for (const auto& character : user.characters) {
                file << character.nama << "|" << character.health << "|" 
                     << character.attack << "|" << character.defense << ";";
            }
            file << "\n";
        }
    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
    }
    file.close();
}

void muatUserData() {
    ifstream file("csvFile/userData.csv");
    try {
        if (!file.is_open()) {
            throw runtime_error("File userData.csv tidak ditemukan!");
        }

        string line;
        getline(file, line); // Skip header
        dataUser.clear();

        while (getline(file, line)) {
            if (line.empty()) continue;

            vector<string> row;
            stringstream ss(line);
            string cell;

            while (getline(ss, cell, ',')) {
                row.push_back(cell);
            }

            if (row.size() < 4) continue; 
            UserData user;
            user.username = row[0];
            user.password = row[1]; 
            user.coins = stoi(row[2]);

            // Parse characters
            if (row.size() >= 4) { 
                stringstream charsStream(row[3]);
                string charData;
                while (getline(charsStream, charData, ';')) {
                    if (charData.empty()) continue;
                    
                    vector<string> charParts;
                    stringstream charStream(charData);
                    string part;
                    while (getline(charStream, part, '|')) {
                        charParts.push_back(part);
                    }

                    if (charParts.size() == 4) {
                        UserCharacter uc;
                        uc.nama = charParts[0];
                        uc.health = stoi(charParts[1]);
                        uc.attack = stoi(charParts[2]);
                        uc.defense = stoi(charParts[3]);
                        user.characters.push_back(uc);
                    }
                }
            }

            dataUser.push_back(user);
        }
    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
    }
    file.close();
}

void loaddataUserSekarang() {
    for (auto& user : dataUser) {
        if (user.username == userSekarang) {
            dataUserSekarang = user;
            return;
        }
    }
    
    // Jika user baru, buat data default
    dataUserSekarang.username = userSekarang;
    
    // Cari password dari array users
    for (int i = 0; i < jumlahUser; i++) {
        if (users[i].username == userSekarang) {
            dataUserSekarang.password = users[i].password;
            break;
        }
    }
    
    dataUserSekarang.coins = 0;
    
    // Tambahkan karakter default (karakter pertama dari CSV)
    if (jumlahKarakter > 0) {
        UserCharacter charDefault;
        charDefault.nama = karakter[0].nama;
        charDefault.health = karakter[0].health;
        charDefault.attack = karakter[0].attack;
        charDefault.defense = karakter[0].defense;
        dataUserSekarang.characters.push_back(charDefault);
    }
    
    dataUser.push_back(dataUserSekarang);
    simpanUserData();
}

// ===================== ERROR HANDLING ===================== 

bool ScanInput(const string& input, int& output) {
    stringstream ss(input);
    ss >> output;

    // Cek apakah input valid (ada angka dan tidak gagal)
    if (ss.fail()) {
        return false;
    }

    return true;
}

// ===================== MUSUH FUNCTIONS ===================== 
void tambahMusuh() {
    system("cls");

    try {

        if (jumlahMusuh >= maks_musuh) {
            throw runtime_error("Data musuh penuh! Maksimal: " + to_string(maks_musuh));
        }
        
        Enemy musuhBaru;
        cout << "=== Tambah Musuh ===";
        cout << "\n\nNama Musuh: ";
        getline(cin >> ws, musuhBaru.nama);

        if (musuhBaru.nama.length() > 15) {
            cout << "[ERROR] Nama terlalu panjang! Maksimal 15 karakter.\n";
            enter();
            return;
        }

        // Cek duplikasi nama
        try {
            for (int i = 0; i < jumlahMusuh; i++) {
                if (musuh[i].nama == musuhBaru.nama) {
                    throw runtime_error("Nama musuh sudah digunakan!");
                }
            }
        } catch (const runtime_error& e) {
            cout << "[ERROR] " << e.what() << endl;
            enter();
            return;
        }

        // Input Attack
        cout << "Attack: ";
        try {
            if (!(cin >> musuhBaru.attack) || musuhBaru.attack < 0) {
                throw runtime_error("Attack harus angka >= 0!");
            }
        } catch (const runtime_error& e) {
            cout << "[ERROR] " << e.what() << endl;
            cin.clear();
            bersihkanBuffer();
            enter();
            return;
        }

        // Input Health
        cout << "Health: ";
        try {
            if (!(cin >> musuhBaru.health) || musuhBaru.health < 0) {
                throw runtime_error("Health harus angka >= 0!");
            }
        } catch (const runtime_error& e) {
            cout << "[ERROR] " << e.what() << endl;
            cin.clear();
            bersihkanBuffer();
            enter();
            return;
        }

        // Input Defense
        cout << "Defense: ";
        try {
            if (!(cin >> musuhBaru.defense) || musuhBaru.defense < 0) {
                throw runtime_error("Defense harus angka >= 0!");
            }
        } catch (const runtime_error& e) {
            cout << "[ERROR] " << e.what() << endl;
            cin.clear();
            bersihkanBuffer();
            enter();
            return;
        }

        bersihkanBuffer();

        musuh[jumlahMusuh++] = musuhBaru;
        simpanMusuhKeCSV();
        cout << "\nMusuh berhasil ditambahkan!\n";
        enter();

    } catch (const exception& e) {
        cerr << "\n[ERROR] " << e.what() << endl;
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

void tampilkanDaftarMusuhCRUD() {
    if (jumlahMusuh == 0) {
        cout << "Tidak ada musuh yang tersimpan.\n";
        return;
    }

    system("cls");
    cout << "\n==================== Daftar Musuh ====================\n" << endl;
    cout << left << setw(5) << "No" 
         << setw(20) << "Nama"
         << setw(10) << "Attack"
         << setw(10) << "Health"
         << setw(10) << "Defense" << endl;
    cout << "------------------------------------------------------------\n";

    for (int i = 0; i < jumlahMusuh; i++) {
        cout << left << setw(5) << i+1
             << setw(20) << musuh[i].nama
             << setw(10) << musuh[i].attack 
             << setw(10) << musuh[i].health
             << setw(10) << musuh[i].defense << endl;
    }
    cout << "------------------------------------------------------------\n";
}

void tampilkanMusuh() {
    
    tampilkanDaftarMusuhCRUD();

    // Tambahkan menu sorting untuk musuh
    int pilihan;
    string input;
    do {
        cout << "\n=== SORT MENU MUSUH ===" << endl;
        cout << "1. Urutkan berdasarkan Nama (Ascending)" << endl;
        cout << "2. Urutkan berdasarkan Attack (Descending)" << endl;
        cout << "3. Kembali" << endl;
        cout << "Pilihan: ";
        getline(cin, input);

        if (!ScanInput(input, pilihan)) {
            tampilkanDaftarMusuhCRUD();
            cout << "\nInput tidak valid! Harap masukkan angka 1-3." << endl;
            continue;
        }

        if (pilihan >= 1 && pilihan <= 2) {
            Enemy sortedMusuh[maks_musuh];
            for (int i = 0; i < jumlahMusuh; i++) {
                sortedMusuh[i] = musuh[i];
            }

            if (pilihan == 1) {
                // Bubble Sort untuk Nama (Ascending)
                for (int i = 0; i < jumlahMusuh-1; i++) {
                    for (int j = 0; j < jumlahMusuh-i-1; j++) {
                        if (sortedMusuh[j].nama > sortedMusuh[j+1].nama) {
                            swap(sortedMusuh[j], sortedMusuh[j+1]);
                        }
                    }
                }
                system("cls");
                cout << "\nMusuh setelah diurutkan (Nama Ascending):\n";
            } else if (pilihan == 2) {
                // Bubble Sort untuk Attack (Descending)
                for (int i = 0; i < jumlahMusuh-1; i++) {
                    for (int j = 0; j < jumlahMusuh-i-1; j++) {
                        if (sortedMusuh[j].attack < sortedMusuh[j+1].attack) {
                            swap(sortedMusuh[j], sortedMusuh[j+1]);
                        }
                    }
                }
                system("cls");
                cout << "\nMusuh setelah diurutkan (Attack Descending):\n";
            }
            
            // Tampilkan hasil sorting
            cout << "------------------------------------------------------------\n";
            for (int i = 0; i < jumlahMusuh; i++) {
                cout << left << setw(5) << i+1
                     << setw(20) << sortedMusuh[i].nama
                     << setw(10) << sortedMusuh[i].attack 
                     << setw(10) << sortedMusuh[i].health
                     << setw(10) << sortedMusuh[i].defense << endl;
            }
            cout << "------------------------------------------------------------\n";
        } else if (pilihan != 3) {
            tampilkanDaftarMusuhCRUD();
            cout << "\nPilihan tidak valid! Harap masukkan angka 1-3." << endl;
        }
    } while (pilihan != 3);
}

void ubahMusuh(int startindeks = 0) {
    try {
        if (jumlahMusuh == 0) {
            cout << "Tidak ada karakter yang tersimpan.\n";
            enter();
            return;
        } 

        // Tampilkan daftar karakter
        tampilkanDaftarMusuhCRUD();

        int indeks;
        cout << "\nMasukkan nomor musuh yang ingin diubah: ";
        try {
            if (!(cin >> indeks) || indeks < startindeks + 1 || indeks > jumlahMusuh) {
                throw runtime_error("Nomor tidak valid!");
            }

            indeks--; 
        } catch (const runtime_error& e) {
            cout << "[ERROR] " << e.what() << endl;
            cin.clear();
            bersihkanBuffer();
            enter();
            return;
        }

        bersihkanBuffer();

        Enemy tempEnemy = musuh[indeks]; // Salin data lama

        cout << "Nama Baru: ";
        getline(cin >> ws, tempEnemy.nama);

        if (tempEnemy.nama.length() > 15) {
            cout << "[ERROR] Nama terlalu panjang! Maksimal 15 karakter.\n";
            enter();
            return;
        }

        // Cek duplikasi nama (selain dirinya sendiri)
        try {
            for (int i = 0; i < jumlahMusuh; i++) {
                if (i != indeks && musuh[i].nama == tempEnemy.nama) {
                    throw runtime_error("Nama sudah digunakan oleh musuh lain!");
                }
            }
        } catch (const runtime_error& e) {
            cout << "[ERROR] " << e.what() << endl;
            enter();
            return;
        }

        cout << "Attack Baru: ";
        try {
            if (!(cin >> tempEnemy.attack) || tempEnemy.attack < 0) {
                throw runtime_error("Attack harus angka >= 0!");
            }

        } catch (const runtime_error& e) {
            cout << "[ERROR] " << e.what() << endl;
            cin.clear();
            bersihkanBuffer();
            enter();
            return;
        }

        cout << "Health Baru: ";
        try {
            if (!(cin >> tempEnemy.health) || tempEnemy.health < 0) {
                throw runtime_error("Health harus angka >= 0!");
            }
        } catch (const runtime_error& e) {
            cout << "[ERROR] " << e.what() << endl;
            cin.clear();
            bersihkanBuffer();
            enter();
            return;
        }

        cout << "Defense Baru: ";
        try {
            if (!(cin >> tempEnemy.defense) || tempEnemy.defense < 0) {
                throw runtime_error("Defense harus angka >= 0!");
            }
        } catch (const runtime_error& e) {
            cout << "[ERROR] " << e.what() << endl;
            cin.clear();
            bersihkanBuffer();
            enter();
            return;
        }

        // Semua input valid → simpan ke musuh
        musuh[indeks] = tempEnemy;
        simpanMusuhKeCSV();
        cout << "Musuh berhasil diubah!\n";
        cin.clear();
        bersihkanBuffer();
        enter();

    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

void hapusMusuh(bool confirm = false, int indeks = -1) {
    if (jumlahMusuh == 0) {
        cout << "Tidak ada musuh yang tersimpan.\n";
        enter();
        return;
    } 
    
    try {
        if (!confirm) {
            tampilkanDaftarMusuhCRUD();
            
            cout << "\nMasukkan nomor musuh yang ingin dihapus: ";
            try {
                if (!(cin >> indeks) || indeks < 1 || indeks > jumlahMusuh) {
                    throw runtime_error("Nomor tidak valid!");
                }

            } catch (const runtime_error& e) {
                cout << "[ERROR] " << e.what() << endl;
                cin.clear();
                bersihkanBuffer(); 
                enter(); 
                return;
            }
            indeks--;

            cout << "Apakah Anda yakin ingin menghapus " << musuh[indeks].nama << "? (y/n): ";
            char pilih;
            cin >> pilih;

            if (pilih == 'y' || pilih == 'Y') {
                hapusMusuh(true, indeks);
            } else {
                cout << "Penghapusan dibatalkan.\n";
                cin.clear();
                bersihkanBuffer(); 
                enter();
                return;
            }
        } else {
            // Proses penghapusan
            for (int i = indeks; i < jumlahMusuh - 1; i++) {
                musuh[i] = musuh[i + 1];
            }
            jumlahMusuh--;
            simpanMusuhKeCSV();
            cout << "Musuh berhasil dihapus!\n";
            cin.clear();
            bersihkanBuffer(); 
            enter();
        }
    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

// ===================== IMPLEMENTASI SORT DAN POINTER ===================== 
// Fungsi dengan parameter dereference
void printKarakterDetail(const Karakter *karakterPtr) {
    system("cls");
    cout << "\nDetail Karakter :" << endl;
    cout << "Nama        : " << karakterPtr->nama << endl;
    cout << "Attack      : " << karakterPtr->attack << endl;
    cout << "Health      : " << karakterPtr->health << endl;
    cout << "Defense     : " << karakterPtr->defense << endl;

    int powerScore = (karakterPtr->attack + karakterPtr->defense) * (karakterPtr->health / 10);
    cout << "Power Score : " << powerScore << endl;
}

// Fungsi dengan parameter address-of
void updateHealth(Karakter &karakterRef, int healthBaru) {
    try {
        if (healthBaru < 0) throw runtime_error("Health tidak boleh negatif!");
        karakterRef.health = healthBaru;
    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
    }
}

// Fungsi untuk menyalin array karakter
void salinKarakter(Karakter dest[], const Karakter src[], int jumlah) {
    for (int i = 0; i < jumlah; i++) {
        dest[i] = src[i];
    }
}

// 1. Bubble Sort untuk Nama (Ascending)
void bubbleSortKarakter(Karakter karakter[], int jumlah) {
    try {
        if (jumlah <= 0) throw runtime_error("Data karakter kosong!");
        
        for (int i = 0; i < jumlah-1; i++) {
            for (int j = 0; j < jumlah-i-1; j++) {
                if (karakter[j].nama > karakter[j+1].nama) {
                    swap(karakter[j], karakter[j+1]);
                }
            }
        }
    } catch (const exception& e) {
        cerr << "[ERROR] Sorting gagal: " << e.what() << endl;
    }
}

// 2. Merge Sort untuk Attack (Descending)
void merge(Karakter karakter[], int kiri, int tengah, int kanan) {
    try {
        // Validasi parameter
        if (kiri < 0 || tengah < kiri || kanan < tengah) {
            throw runtime_error("Indeks tidak valid (kiri=" + to_string(kiri) + 
                              ", tengah=" + to_string(tengah) + 
                              ", kanan=" + to_string(kanan) + ")");
        }

        int ukuranKiri = tengah - kiri + 1;
        int ukuranKanan = kanan - tengah;

        // Validasi ukuran array
        if (ukuranKiri <= 0 || ukuranKanan <= 0) {
            throw runtime_error("Ukuran sub-array tidak valid!");
        }

        Karakter arrayKiri[ukuranKiri], arrayKanan[ukuranKanan];

        // Salin data ke sub-array kiri
        for (int i = 0; i < ukuranKiri; i++) {
            arrayKiri[i] = karakter[kiri + i];
        }

        // Salin data ke sub-array kanan
        for (int j = 0; j < ukuranKanan; j++) {
            arrayKanan[j] = karakter[tengah + 1 + j];
        }

        // Proses merge
        int i = 0, j = 0, k = kiri;
        while (i < ukuranKiri && j < ukuranKanan) {
            if (arrayKiri[i].attack >= arrayKanan[j].attack) {
                karakter[k] = arrayKiri[i];
                i++;
            } else {
                karakter[k] = arrayKanan[j];
                j++;
            }
            k++;
        }

        // Salin sisa elemen jika ada
        while (i < ukuranKiri) {
            karakter[k] = arrayKiri[i];
            i++;
            k++;
        }

        while (j < ukuranKanan) {
            karakter[k] = arrayKanan[j];
            j++;
            k++;
        }

    } catch (const exception& e) {
        cerr << "[ERROR] Merge gagal: " << e.what() << endl;
    }
}

void mergeSortKarakter(Karakter karakter[], int kiri, int kanan) {
    try {
        // Validasi parameter
        if (kiri < 0 || kanan < 0) {
            throw runtime_error("Indeks tidak valid (kiri=" + to_string(kiri) + 
                              ", kanan=" + to_string(kanan) + ")");
        }

        if (jumlahKarakter == 0) {
            throw runtime_error("Array karakter kosong!");
        }

        // Proses rekursif
        if (kiri < kanan) {
            int tengah = kiri + (kanan - kiri) / 2;
            mergeSortKarakter(karakter, kiri, tengah);
            mergeSortKarakter(karakter, tengah + 1, kanan);
            merge(karakter, kiri, tengah, kanan);
        }

    } catch (const exception& e) {
        cerr << "[ERROR] MergeSort gagal: " << e.what() << endl;
    }
}


// ===================== MAIN MENU FUNCTION ===================== 
void login(int maks_percobaan = maks_attempt) {
    int attempt = 0;
    while (attempt < maks_percobaan) {
        system("cls");
        try {
            string username, password;
            cout << "=== LOGIN ===" << endl;
            cout << "Percobaan ke-" << attempt + 1 << " dari " << maks_percobaan << endl;
            
            cout << "Username: ";
            if (!(cin >> username)) throw runtime_error("Input username tidak valid!");
            
            cout << "Password: ";
            if (!(cin >> password)) throw runtime_error("Input password tidak valid!");

            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            if (username == "demonia" && password == "utakatik") {
                is_login = true;
                userSekarang = username;
                is_admin = true;
                cout << "Login sebagai admin berhasil!\n";
                return;
            }

            bool found = false;
            for (const auto& user : dataUser) {
                if (user.username == username && user.password == password) {
                    found = true;
                    is_login = true;
                    userSekarang = username;
                    loaddataUserSekarang(); 
                    cout << "Login berhasil!\n";
                    return;
                }
            }
            if (!found) throw runtime_error("Username/password salah!\nTekan enter..");
            
        } catch (const exception& e) {
            cerr << "\n\n[ERROR] " << e.what() << endl;
            if (++attempt >= maks_percobaan) {
                cerr << "Anda telah gagal login " << maks_percobaan << " kali. Program berhenti.\n";
                exit(1);
            }
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
}

void registrasi(int maxUsers = maks_user) {
    system("cls");
    try {
        if (jumlahUser >= maxUsers) {
            throw runtime_error("Kapasitas user penuh! Tidak bisa mendaftar.");
        }

        string username, password;
        cout << "\n=== REGISTER ===" << endl;
        cout << "Username: ";
        if (!(cin >> username) || username.empty()) {
            throw runtime_error("Username tidak boleh kosong!");
        }

        // Cek duplikasi username
        try {
            for (const auto& user : dataUser) {
                if (user.username == username) {
                    throw runtime_error("Username sudah digunakan!");
                }
            }
        } catch (const runtime_error& e) {
            cout << "[ERROR] " << e.what() << endl;
            bersihkanBuffer();
            enter();
            return; 
        }

        try {
            for (int i = 0; i < jumlahUser; i++) {
                if (users[i].username == username) {
                    throw runtime_error("Username sudah digunakan");
                }
            }
        } catch (const runtime_error& e) {
            cout << "[ERROR] " << e.what() << endl;
            bersihkanBuffer();
            enter(); 
            return; 
        }


        cout << "Password: ";
        if (!(cin >> password) || password.empty()) {
            throw runtime_error("Password tidak boleh kosong!");
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // Buat user baru
        UserData userBaru;
        userBaru.username = username;
        userBaru.password = password;
        userBaru.coins = 0;
        
        // Tambahkan karakter default jika ada
        if (jumlahKarakter > 0) {
            UserCharacter charDefault;
            charDefault.nama = karakter[0].nama;
            charDefault.health = karakter[0].health;
            charDefault.attack = karakter[0].attack;
            charDefault.defense = karakter[0].defense;
            userBaru.characters.push_back(charDefault);
        }
        
        dataUser.push_back(userBaru);
        simpanUserData();

        cout << "Registrasi berhasil! Silakan login.\n";
    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
        cin.clear(); 
        cin.ignore(1000, '\n'); 
    }
}

// ===================== CHARACTER FUNCTION ===================== 

void tambahKarakter(int maksKarakter = maks_karakter) {
    try {
        if (jumlahKarakter >= maksKarakter) {
            throw runtime_error("Data karakter penuh! Maksimal: " + to_string(maksKarakter));
        }

        Karakter newKarakter;
        system("cls");
        cout << "=== Tambah Karakter ===";
        cout << "\n\nNama Karakter: ";
        getline(cin >> ws, newKarakter.nama);

        if (newKarakter.nama.length() > 15) {
            cout << "[ERROR] Nama terlalu panjang! Maksimal 15 karakter.\n";
            enter();           
            return;
        }

        // Cek duplikasi nama
        for (int i = 0; i < jumlahKarakter; i++) {
            if (karakter[i].nama == newKarakter.nama) {
                throw runtime_error("Nama karakter sudah digunakan!\n\nTekan Enter untuk kembali ke menu...");
            }
        }

        cout << "Attack: ";
        try {
            if (!(cin >> newKarakter.attack) || newKarakter.attack < 0) {
                throw runtime_error("\nAttack harus angka >= 0!");
            }
        } catch (const runtime_error& e) {
            cout << e.what() << endl;
            cin.clear();
            bersihkanBuffer();
            enter();
            return;
        }

        cout << "Health: ";
        try {
            if (!(cin >> newKarakter.health) || newKarakter.health < 0) {
                throw runtime_error("\nHealth harus angka >= 0!");
            }
        } catch (const runtime_error& e) {
            cout << e.what() << endl;
            cin.clear();
            bersihkanBuffer();
            enter();
            return;
        }

        cout << "Defense: ";
        try {
            if (!(cin >> newKarakter.defense) || newKarakter.defense < 0) {
                throw runtime_error("\nDefense harus angka >= 0!");
            }
        } catch (const runtime_error& e) {
            cout << e.what() << endl;
            cin.clear();
            bersihkanBuffer();
            enter();
            return;
        }

        karakter[jumlahKarakter++] = newKarakter;
        simpanKeCSV();
        cout << "\nKarakter berhasil ditambahkan!\n";
        cin.clear();
        bersihkanBuffer();
        enter();

    } catch (const exception& e) {
        cerr << "\n[ERROR] " << e.what() << endl;
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

void tampilkanKarakterCRUD(){
    system("cls");
    cout << "\n==================== Daftar Karakter ====================\n"<< endl;
        cout << left << setw(5) << "No" 
             << setw(20) << "Nama"
             << setw(10) << "Attack"
             << setw(10) << "Health"
             << setw(10) << "Defense" << endl;
        cout << "------------------------------------------------------------\n";

        for (int i = 0; i < jumlahKarakter; i++) {
            cout << left << setw(5) << i+1
                 << setw(20) << karakter[i].nama
                 << setw(10) << karakter[i].attack 
                 << setw(10) << karakter[i].health
                 << setw(10) << karakter[i].defense << endl;
        }
        cout << "------------------------------------------------------------\n";
}

void tampilkanKarakter(bool detailed = false) {
    if (jumlahKarakter == 0) {
        cout << endl << "Tidak ada karakter yang tersimpan.\n";
        enter();
        return;
    }

    if (!detailed) {
        tampilkanKarakterCRUD();
        
    } else {
        tampilkanKarakterCRUD();

        string lihatDetailInput;
        cout << "\nLihat detail salah satu karakter? (y/n): ";
        getline(cin, lihatDetailInput);

        if (lihatDetailInput.length() == 1 && (lihatDetailInput[0] == 'y' || lihatDetailInput[0] == 'Y')) {
            string indexInput;
            int indexDetail;
            cout << "Masukkan nomor karakter: ";
            getline(cin, indexInput);

            if (ScanInput(indexInput, indexDetail) && indexDetail >= 1 && indexDetail <= jumlahKarakter) {
                printKarakterDetail(&karakter[indexDetail - 1]);
            } else {
                cout << "[ERROR] Nomor karakter tidak valid.\n";
            }
            enter();
            return;

        } else if (lihatDetailInput.length() == 1 && (lihatDetailInput[0] == 'n' || lihatDetailInput[0] == 'N')) {
            // Tidak ingin lihat detail — kembali saja
            return;

        } else {
            cout << "[ERROR] Input tidak valid.";
            enter();
            return;
        }
        
    }

    // sort menu 
    int pilihan;
    string input;
    do {
        cout << "\n=== SORT MENU ===" << endl;
        cout << "1. Urutkan berdasarkan Nama (Ascending)" << endl;
        cout << "2. Urutkan berdasarkan Attack (Descending)" << endl;
        cout << "3. Kembali ke Menu Utama" << endl;
        cout << "Pilihan: ";

        getline(cin, input);

        if (!ScanInput(input, pilihan)) {
            tampilkanKarakterCRUD();
            cout << "\nInput tidak valid! Masukkan angka antara 1-3." << endl;
            continue;
        }

        if (pilihan >= 1 && pilihan <= 2) {
            Karakter sortedKarakter[maks_karakter];
            salinKarakter(sortedKarakter, karakter, jumlahKarakter);
            
            if (pilihan == 1) {
                bubbleSortKarakter(sortedKarakter, jumlahKarakter);
                system("cls");
                cout << "\nKarakter setelah diurutkan (Nama Ascending):\n";
            } else if (pilihan == 2) {
                mergeSortKarakter(sortedKarakter, 0, jumlahKarakter-1);
                system("cls");
                cout << "\nKarakter setelah diurutkan (Attack Descending):\n";
            }
            
            // Tampilkan hasil sorting
            cout << "\n==================== Daftar Karakter (Sorted) ====================\n"<< endl;
            cout << left << setw(5) << "No" 
                << setw(20) << "Nama"
                << setw(10) << "Attack"
                << setw(10) << "Health"
                << setw(10) << "Defense" << endl;
            cout << "------------------------------------------------------------\n";

            for (int i = 0; i < jumlahKarakter; i++) {
                cout << left << setw(5) << i+1
                    << setw(20) << sortedKarakter[i].nama
                    << setw(10) << sortedKarakter[i].attack 
                    << setw(10) << sortedKarakter[i].health
                    << setw(10) << sortedKarakter[i].defense << endl;
            }
            cout << "------------------------------------------------------------\n";
        } 
        
        else if (pilihan != 3) {
            tampilkanKarakterCRUD();
            cout << "\nPilihan tidak valid! Masukkan angka antara 1-3." << endl;
        }
    } while (pilihan != 3);
}

void ubahKarakter(int startindeks = 0) {
    try {
        if (jumlahKarakter == 0) {
            cout << "Tidak ada karakter yang tersimpan.\n";
            enter();
            return;
        } 

        // Tampilkan daftar karakter
        tampilkanKarakterCRUD();

        int indeks;
        cout << "Masukkan nomor karakter yang ingin diubah: ";

        try {
            if (!(cin >> indeks) || indeks < startindeks + 1 || indeks > jumlahKarakter) {
                throw runtime_error("Nomor tidak valid!");
            }
            indeks--;
        } catch (const runtime_error& e) {
            cout << "[ERROR] " << e.what() << endl;
            cin.clear();         // Reset status error input
            bersihkanBuffer();   // Buang sisa input di buffer
            enter();
            return;
        }

        // Input data baru
        string namaBaru;
        cout << "Nama Baru: ";
        getline(cin >> ws, namaBaru);

        if (namaBaru.length() > 15) {
            cout << "[ERROR] Nama terlalu panjang! Maksimal 15 karakter.\n";
            enter();
            return;
        }

        try {
            for (int i = 0; i < jumlahKarakter; i++) {
                if (i != indeks && karakter[i].nama == namaBaru) {
                    throw runtime_error("Nama sudah digunakan oleh karakter lain!");
                }
            }
        } catch (const runtime_error& e) {
            cout << "[ERROR] " << e.what() << endl;
            cin.clear();
            return;
        }

        karakter[indeks].nama = namaBaru;

        cout << "Attack Baru: ";
        try {
            if (!(cin >> karakter[indeks].attack) || karakter[indeks].attack < 0) {
                throw runtime_error("\nAttack harus angka >= 0!");
            }
        } catch (const runtime_error& e) {
            cout << e.what() << endl;
            cin.clear();
            bersihkanBuffer();
            enter();
            return;
        }

        cout << "Health Baru: ";
        int healthBaru;
        try {
            if (!(cin >> healthBaru)) {
                throw runtime_error("\nHealth harus angka >= 0!");
            }

            updateHealth(karakter[indeks], healthBaru);

        } catch (const runtime_error& e) {
            cout << e.what() << endl;
            cin.clear();
            bersihkanBuffer();
            enter();
            return;
        }

        cout << "Defense Baru: ";
        try {
            if (!(cin >> karakter[indeks].defense) || karakter[indeks].defense < 0) {
                throw runtime_error("\nDefense harus angka >= 0!");
            }
        } catch (const runtime_error& e) {
            cout << e.what() << endl;
            cin.clear();
            bersihkanBuffer();
            enter();
            return;
        }

        bersihkanBuffer();

        simpanKeCSV();
        cout << "Karakter berhasil diubah!\n";
        enter();

    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

void hapusKarakter(bool confirm = false, int indeks = -1) {
    if (jumlahKarakter == 0) {
        cout << "Tidak ada karakter yang tersimpan.\n";
        enter();
        return;
    } 
    
    try {
        if (!confirm) {
            tampilkanKarakterCRUD();
            
            cout << "Masukkan nomor karakter yang ingin dihapus: ";
            try {
                if (!(cin >> indeks) || indeks < 1 || indeks > jumlahKarakter) {
                    throw runtime_error("Nomor tidak valid!");
                }
            } catch (const exception& e) {
                cerr << "\n[ERROR] " << e.what() << endl;
                cin.clear();
                bersihkanBuffer();
                enter();
                return;
            }
            indeks--;

            cout << "Apakah Anda yakin ingin menghapus " << karakter[indeks].nama << "? (y/n): ";
            char pilih;
            cin >> pilih;
            cin.clear();
            bersihkanBuffer();

            if (pilih == 'y' || pilih == 'Y') {
                hapusKarakter(true, indeks);
            } else {
                cout << "Penghapusan dibatalkan.\n";
                enter();
                return;
            }
        } else {
            // Proses penghapusan
            for (int i = indeks; i < jumlahKarakter - 1; i++) {
                karakter[i] = karakter[i + 1];
            }
            jumlahKarakter--;
            simpanKeCSV();
            cout << "Karakter berhasil dihapus!\n";
            enter();
        }
    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
        cin.clear();
    }
}


//========================== SEARCHING ==========================
// Linear Search untuk Karakter (berdasarkan nama)
void linearSearchKarakter(const string& nama) {
    try {
        if (jumlahKarakter == 0) throw runtime_error("Tidak ada karakter yang tersimpan!");
        
        system("cls");
        bool found = false;
        cout << "\n=== HASIL PENCARIAN KARAKTER ===" << endl;
        cout << "------------------------------------------------------------" << endl;
        cout << left << setw(20) << "Nama" 
             << setw(10) << "Attack" 
             << setw(10) << "Health" 
             << setw(10) << "Defense" << endl;
        cout << "------------------------------------------------------------" << endl;
        
        for (int i = 0; i < jumlahKarakter; i++) {
            if (karakter[i].nama.find(nama) != string::npos) {
                cout << left << setw(20) << karakter[i].nama 
                     << setw(10) << karakter[i].attack
                     << setw(10) << karakter[i].health
                     << setw(10) << karakter[i].defense << endl;
                found = true;
            }
        }
        
        
        if (!found) {
            cout << "Tidak ditemukan karakter dengan nama mengandung '" << nama << "'" << endl;
        }
        cout << "------------------------------------------------------------" << endl;

        cin.clear();
        enter();
        
    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
    }
}

// Linear Search untuk Musuh (berdasarkan nama)
void linearSearchMusuh(const string& nama) {
    try {
        if (jumlahMusuh == 0) throw runtime_error("Tidak ada musuh yang tersimpan!");
        
        system("cls");
        bool found = false;
        cout << "\n=== HASIL PENCARIAN MUSUH ===" << endl;
        cout << "------------------------------------------------------------" << endl;
        cout << left << setw(20) << "Nama" << setw(10) << "Attack" 
             << setw(10) << "Health" << setw(10) << "Defense" << endl;
        cout << "------------------------------------------------------------" << endl;
        
        for (int i = 0; i < jumlahMusuh; i++) {
            if (musuh[i].nama.find(nama) != string::npos) {
                cout << left << setw(20) << musuh[i].nama 
                     << setw(10) << musuh[i].attack
                     << setw(10) << musuh[i].health
                     << setw(10) << musuh[i].defense << endl;
                found = true;
            }
        }
        
        if (!found) {
            cout << "Tidak ditemukan musuh dengan nama mengandung '" << nama << "'" << endl;
        }
        cout << "------------------------------------------------------------" << endl;

        enter();
    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
    }
}

// Binary Search untuk Karakter (bisa untuk attack, health, atau defense)
void binarySearchKarakter(int searchnilai, SearchType type) {
    try {
        if (jumlahKarakter == 0) throw runtime_error("Tidak ada karakter yang tersimpan!");
        
        // Urutkan dulu untuk binary search
        Karakter sortedKarakter[maks_karakter];
        salinKarakter(sortedKarakter, karakter, jumlahKarakter);
        mergeSortKarakter(sortedKarakter, 0, jumlahKarakter-1); // Urutkan descending
        
        int low = 0;
        int high = jumlahKarakter - 1;
        bool found = false;
        
        string kolomPencarian;
        switch(type) {
            case ATTACK: kolomPencarian = "Attack"; break;
            case HEALTH: kolomPencarian = "Health"; break;
            case DEFENSE: kolomPencarian = "Defense"; break;
        }

        system("cls"); 

        cout << "\n=== HASIL PENCARIAN KARAKTER (Binary Search - " << kolomPencarian << ": " << searchnilai << ") ===" << endl;
        cout << "------------------------------------------------------------" << endl;
        cout << left << setw(20) << "Nama" << setw(10) << "Attack" 
             << setw(10) << "Health" << setw(10) << "Defense" << endl;
        cout << "------------------------------------------------------------" << endl;
        
        while (low <= high) {
            int mid = low + (high - low) / 2;
            int currentnilai;
            
            switch(type) {
                case ATTACK: currentnilai = sortedKarakter[mid].attack; break;
                case HEALTH: currentnilai = sortedKarakter[mid].health; break;
                case DEFENSE: currentnilai = sortedKarakter[mid].defense; break;
            }
            
            if (currentnilai == searchnilai) {
                // Tampilkan hanya yang nilainya sama persis
                cout << left << setw(20) << sortedKarakter[mid].nama 
                     << setw(10) << sortedKarakter[mid].attack
                     << setw(10) << sortedKarakter[mid].health
                     << setw(10) << sortedKarakter[mid].defense << endl;
                found = true;
                
                // Cari ke kiri untuk nilai yang sama
                int left = mid - 1;
                while (left >= 0) {
                    switch(type) {
                        case ATTACK: currentnilai = sortedKarakter[left].attack; break;
                        case HEALTH: currentnilai = sortedKarakter[left].health; break;
                        case DEFENSE: currentnilai = sortedKarakter[left].defense; break;
                    }
                    if (currentnilai == searchnilai) {
                        cout << left << setw(20) << sortedKarakter[left].nama 
                             << setw(10) << sortedKarakter[left].attack
                             << setw(10) << sortedKarakter[left].health
                             << setw(10) << sortedKarakter[left].defense << endl;
                        left--;
                    } else {
                        break;
                    }
                }
                
                // Cari ke kanan untuk nilai yang sama
                int right = mid + 1;
                while (right < jumlahKarakter) {
                    switch(type) {
                        case ATTACK: currentnilai = sortedKarakter[right].attack; break;
                        case HEALTH: currentnilai = sortedKarakter[right].health; break;
                        case DEFENSE: currentnilai = sortedKarakter[right].defense; break;
                    }
                    if (currentnilai == searchnilai) {
                        cout << left << setw(20) << sortedKarakter[right].nama 
                             << setw(10) << sortedKarakter[right].attack
                             << setw(10) << sortedKarakter[right].health
                             << setw(10) << sortedKarakter[right].defense << endl;
                        right++;
                    } else {
                        break;
                    }
                }
                break;
            }
            
            if (currentnilai < searchnilai) {
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }
        
        if (!found) {
            cout << "Tidak ditemukan karakter dengan " << kolomPencarian << " = " << searchnilai << endl;
        }
        cout << "------------------------------------------------------------" << endl;

        enter();
    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
    }
}

// Binary Search untuk Musuh (bisa untuk attack, health, atau defense)
void binarySearchMusuh(int searchnilai, SearchType type) {
    try {
        if (jumlahMusuh == 0) throw runtime_error("Tidak ada musuh yang tersimpan!");
        
        // Urutkan dulu untuk binary search
        Enemy sortedMusuh[maks_musuh];
        for (int i = 0; i < jumlahMusuh; i++) {
            sortedMusuh[i] = musuh[i];
        }
        
        // Bubble sort berdasarkan tipe yang dipilih
        for (int i = 0; i < jumlahMusuh-1; i++) {
            for (int j = 0; j < jumlahMusuh-i-1; j++) {
                bool shouldSwap = false;
                switch(type) {
                    case ATTACK: 
                        shouldSwap = (sortedMusuh[j].attack < sortedMusuh[j+1].attack);
                        break;
                    case HEALTH:
                        shouldSwap = (sortedMusuh[j].health < sortedMusuh[j+1].health);
                        break;
                    case DEFENSE:
                        shouldSwap = (sortedMusuh[j].defense < sortedMusuh[j+1].defense);
                        break;
                }
                if (shouldSwap) {
                    swap(sortedMusuh[j], sortedMusuh[j+1]);
                }
            }
        }
        
        int low = 0;
        int high = jumlahMusuh - 1;
        bool found = false;
        
        string kolomPencarian;
        switch(type) {
            case ATTACK: kolomPencarian = "Attack"; break;
            case HEALTH: kolomPencarian = "Health"; break;
            case DEFENSE: kolomPencarian = "Defense"; break;
        }
        
        system("cls");
        cout << "\n=== HASIL PENCARIAN MUSUH (Binary Search - " << kolomPencarian << ": " << searchnilai << ") ===" << endl;
        cout << "------------------------------------------------------------" << endl;
        cout << left << setw(20) << "Nama" << setw(10) << "Attack" 
             << setw(10) << "Health" << setw(10) << "Defense" << endl;
        cout << "------------------------------------------------------------" << endl;
        
        while (low <= high) {
            int mid = low + (high - low) / 2;
            int currentnilai;
            
            switch(type) {
                case ATTACK: currentnilai = sortedMusuh[mid].attack; break;
                case HEALTH: currentnilai = sortedMusuh[mid].health; break;
                case DEFENSE: currentnilai = sortedMusuh[mid].defense; break;
            }
            
            if (currentnilai == searchnilai) {
                // Tampilkan semua yang sama
                int temp = mid;
                while (temp >= 0) {
                    switch(type) {
                        case ATTACK: if (sortedMusuh[temp].attack != searchnilai) break; break;
                        case HEALTH: if (sortedMusuh[temp].health != searchnilai) break; break;
                        case DEFENSE: if (sortedMusuh[temp].defense != searchnilai) break; break;
                    }
                    cout << left << setw(20) << sortedMusuh[temp].nama 
                         << setw(10) << sortedMusuh[temp].attack
                         << setw(10) << sortedMusuh[temp].health
                         << setw(10) << sortedMusuh[temp].defense << endl;
                    temp--;
                    found = true;
                }
                temp = mid + 1;
                while (temp < jumlahMusuh) {
                    switch(type) {
                        case ATTACK: if (sortedMusuh[temp].attack != searchnilai) break; break;
                        case HEALTH: if (sortedMusuh[temp].health != searchnilai) break; break;
                        case DEFENSE: if (sortedMusuh[temp].defense != searchnilai) break; break;
                    }
                    cout << left << setw(20) << sortedMusuh[temp].nama 
                         << setw(10) << sortedMusuh[temp].attack
                         << setw(10) << sortedMusuh[temp].health
                         << setw(10) << sortedMusuh[temp].defense << endl;
                    temp++;
                    found = true;
                }
                break;
            }
            
            if (currentnilai < searchnilai) {
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }
        
        if (!found) {
            cout << "Tidak ditemukan musuh dengan " << kolomPencarian << " = " << searchnilai << endl;
        }
        cout << "------------------------------------------------------------" << endl;
        enter();

    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
    }
}

void menuSearchKarakter() {
    int metodeSearch;
    string input;
    tampilkanKarakterCRUD();
    
    cout << "\n=== PENCARIAN KARAKTER ===" << endl;
    cout << "1. Cari berdasarkan Nama (Linear Search)" << endl;
    cout << "2. Cari berdasarkan Attack (Binary Search)" << endl;
    cout << "3. Cari berdasarkan Health (Binary Search)" << endl;
    cout << "4. Cari berdasarkan Defense (Binary Search)" << endl;
    cout << "5. Kembali" << endl;
    cout << "Pilihan: ";
    getline(cin, input);

    if (!ScanInput(input, metodeSearch)) {
        cout << "Input tidak valid! Masukkan angka antara 1-5." << endl;
        enter();
        return;
    }
    
    if (metodeSearch == 1) {
        string nama;
        cout << "Masukkan nama (atau bagian nama) yang dicari: ";
        getline(cin, nama);
        linearSearchKarakter(nama);
    } else if (metodeSearch >= 2 && metodeSearch <= 4) {
        int nilai;
        string kolom, nilaiInput;
        switch(metodeSearch) {
            case 2: kolom = "attack"; break;
            case 3: kolom = "health"; break;
            case 4: kolom = "defense"; break;
        }
        cout << "Masukkan nilai " << kolom << " yang dicari: ";
        getline(cin, nilaiInput);

        if (!ScanInput(nilaiInput, nilai)) {
            cout << "Input tidak valid! Masukkan angka untuk " << kolom << "." << endl;
            enter();
            return;
        }
        
        SearchType type;
        switch(metodeSearch) {
            case 2: type = ATTACK; break;
            case 3: type = HEALTH; break;
            case 4: type = DEFENSE; break;
        }
        binarySearchKarakter(nilai, type);
    } else if (metodeSearch != 5) {
        cout << "Pilihan tidak valid! Masukkan angka antara 1-5." << endl;
        enter();
    }
}

void menuSearchMusuh() {
    int metodeSearch;
    string input;
    system("cls");
    tampilkanDaftarMusuhCRUD();

    cout << "\n=== PENCARIAN MUSUH ===" << endl;
    cout << "1. Cari berdasarkan Nama (Linear Search)" << endl;
    cout << "2. Cari berdasarkan Attack (Binary Search)" << endl;
    cout << "3. Cari berdasarkan Health (Binary Search)" << endl;
    cout << "4. Cari berdasarkan Defense (Binary Search)" << endl;
    cout << "5. Kembali" << endl;
    cout << "Pilihan: ";
    getline(cin, input);

    if (!ScanInput(input, metodeSearch)) {
        cout << "Input tidak valid! Masukkan angka antara 1-5." << endl;
        enter();
        return;
    }
    
    if (metodeSearch == 1) {
        string nama;
        cout << "Masukkan nama (atau bagian nama) yang dicari: ";
        getline(cin, nama);
        linearSearchMusuh(nama);
    } else if (metodeSearch >= 2 && metodeSearch <= 4) {
        int nilai;
        string kolom, nilaiInput;
        switch(metodeSearch) {
            case 2: kolom = "attack"; break;
            case 3: kolom = "health"; break;
            case 4: kolom = "defense"; break;
        }
        cout << "Masukkan nilai " << kolom << " yang dicari: ";
        getline(cin, nilaiInput);

        if (!ScanInput(nilaiInput, nilai)) {
            cout << "Input tidak valid! Masukkan angka untuk " << kolom << "." << endl;
            enter();
            return;
        }
        
        SearchType type;
        switch(metodeSearch) {
            case 2: type = ATTACK; break;
            case 3: type = HEALTH; break;
            case 4: type = DEFENSE; break;
        }
        binarySearchMusuh(nilai, type);
    } else if (metodeSearch != 5) {
        cout << "Pilihan tidak valid! Masukkan angka antara 1-5." << endl;
        enter();
    }
}

void menuSearch() {
    int searchpilih;
    string input;

    system("cls");
    cout << "\n=== MENU PENCARIAN ===" << endl;
    cout << "1. Cari Karakter" << endl;
    cout << "2. Cari Musuh" << endl;
    cout << "3. Kembali" << endl;
    cout << "Pilihan: ";
    getline(cin, input);

    if (!ScanInput(input, searchpilih)) {
        cout << "Input tidak valid! Harap masukkan angka 1-3." << endl;
        enter();
        return;
    }
    
    switch(searchpilih) {
        case 1: 
            menuSearchKarakter();
            break;
        case 2:
            menuSearchMusuh();
            break;
        case 3:
            break;
        default:
            cout << "Pilihan tidak valid! Harap masukkan angka 1-3." << endl;
            enter();
    }
}

// ===================== MENU KELOLA KARAKTER =====================
void kelolaKarakter() {
    int pilihan;
    string input;
    
    while (true) {
        system("cls");
        cout << "=== KELOLA KARAKTER ===" << endl;
        cout << "1. Tambah Karakter" << endl;
        cout << "2. Tampilkan Karakter" << endl;
        cout << "3. Ubah Karakter" << endl;
        cout << "4. Hapus Karakter" << endl;
        cout << "5. Kembali ke Menu Admin" << endl;
        cout << "Pilihan: ";
        getline(cin, input);

        if (!ScanInput(input, pilihan)) {
            cout << "Input tidak valid! Harap masukkan angka 1-5." << endl;
            enter();
            continue;
        }

        switch (pilihan) {
            case 1: tambahKarakter(); break;
            case 2: tampilkanKarakter(); break;
            case 3: ubahKarakter(); break;
            case 4: hapusKarakter(); break;
            case 5: return; // Kembali ke menu admin
            default:
                cout << "Pilihan tidak valid!" << endl;
        }
    }
}

// ===================== MENU KELOLA MUSUH =====================
void kelolaMusuh() {
    int pilihan;
    string input;
    
    while (true) {
        system("cls");
        cout << "=== KELOLA MUSUH ===" << endl;
        cout << "1. Tambah Musuh" << endl;
        cout << "2. Tampilkan Musuh" << endl;
        cout << "3. Ubah Musuh" << endl;
        cout << "4. Hapus Musuh" << endl;
        cout << "5. Kembali ke Menu Admin" << endl;
        cout << "Pilihan: ";
        getline(cin, input);

        if (!ScanInput(input, pilihan)) {
            cout << "Input tidak valid! Harap masukkan angka 1-5." << endl;
            enter();
            continue;
        }

        switch (pilihan) {
            case 1: tambahMusuh(); break;
            case 2: tampilkanMusuh(); break;
            case 3: ubahMusuh(); break;
            case 4: hapusMusuh(); break;
            case 5: return; 
            default:
                cout << "Pilihan tidak valid!" << endl;
        }
    }
}

// ===================== FUNGSI KELOLA USER =====================
void tampilkanUser() {
    if (dataUser.empty()) {
        cout << "Tidak ada user yang tersimpan.\n";
        enter();
        return;
    }

    cout << "\n==================== Daftar User ====================\n"<< endl;
    cout << left << setw(5) << "No" 
         << setw(20) << "Username"
         << setw(15) << "Password"
         << setw(10) << "Coins"
         << setw(15) << "Jumlah Karakter" << endl;
    cout << "------------------------------------------------------------\n";

    for (int i = 0; i < dataUser.size(); i++) {
        cout << left << setw(5) << i+1
             << setw(20) << dataUser[i].username
             << setw(15) << dataUser[i].password
             << setw(10) << dataUser[i].coins
             << setw(15) << dataUser[i].characters.size() << endl;
    }
    cout << "------------------------------------------------------------\n";
}

void hapusUser() {
    try {
        if (dataUser.empty()) {
            cout << "Tidak ada user yang tersimpan.\n";
            enter();
            return;
        }

        tampilkanUser();
        
        int index;
        string input;
        cout << "Masukkan nomor user yang ingin dihapus (0 untuk kembali): ";
        getline(cin, input);
        
        if (!ScanInput(input, index)) {
            throw runtime_error("Input tidak valid!");
        }

        if (index == 0) return; // Kembali jika memilih 0
        
        if (index < 1 || index > dataUser.size()) {
            throw runtime_error("Nomor tidak valid!");
        }
        index--; // Konversi ke index 0-based

        // Konfirmasi penghapusan
        cout << "Apakah Anda yakin ingin menghapus user " << dataUser[index].username << "? (y/n): ";
        char choice;
        cin >> choice;
        bersihkanBuffer();

        if (choice == 'y' || choice == 'Y') {
            // Jangan izinkan menghapus admin utama
            if (dataUser[index].username == "demonia") {
                throw runtime_error("Tidak bisa menghapus admin utama!");
            }

            // Hapus user dari vector
            dataUser.erase(dataUser.begin() + index);
            
            // Simpan perubahan ke CSV
            simpanUserData();
            cout << "User berhasil dihapus!\n";
        } else {
            cout << "Penghapusan dibatalkan.\n";
        }
    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

void kelolaUser() {
    int pilihan;
    string input;
    
    while (true) {
        system("cls");
        cout << "\n=== KELOLA USER ===" << endl;
        cout << "1. Tampilkan User" << endl;
        cout << "2. Hapus User" << endl;
        cout << "3. Kembali ke Menu Admin" << endl;
        cout << "Pilihan: ";
        getline(cin, input);

        if (!ScanInput(input, pilihan)) {
            cout << "Input tidak valid! Harap masukkan angka 1-3." << endl;
            enter();
            continue;
        }

        switch (pilihan) {
            case 1: tampilkanUser(); break;
            case 2: hapusUser(); break;
            case 3: return; // Kembali ke menu admin
            default:
                cout << "Pilihan tidak valid!" << endl;
        }
    }
}

// ===================== BATTLE SYSTEM =====================
void tampilkanStatusBattle(const UserCharacter& player, const Enemy& enemy, int turn) {
    system("cls");
    cout << "\n=== TURN " << turn << " ===" << endl;
    cout << "----------------------------------------\n";
    cout << "| " << left << setw(20) << player.nama << " vs " << setw(20) << enemy.nama << " |\n";
    cout << "----------------------------------------\n";
    cout << "| HEALTH: " << setw(5) << player.health << " | " << setw(5) << enemy.health << " |\n";
    cout << "| ATTACK: " << setw(5) << player.attack << " | " << setw(5) << enemy.attack << " |\n";
    cout << "| DEFENSE:" << setw(5) << player.defense << " | " << setw(5) << enemy.defense << " |\n";
    cout << "----------------------------------------\n\n";
}

void battle() {
    try {
        if (dataUserSekarang.characters.empty()) {
            throw runtime_error("Anda tidak memiliki karakter untuk bertarung!");
        }

        if (jumlahMusuh == 0) {
            throw runtime_error("Tidak ada musuh yang tersedia untuk bertarung!");
        }

        // Pilih musuh acak
        int randomEnemyindeks = rand() % jumlahMusuh;
        Enemy enemy = musuh[randomEnemyindeks];
        
        system("cls");
        cout << "\n=== ANDA AKAN MELAWAN ===\n";
        cout << "Nama: " << enemy.nama << endl;
        cout << "Health: " << enemy.health << endl;
        cout << "Attack: " << enemy.attack << endl;
        cout << "Defense: " << enemy.defense << endl;
        cout << "--------------------------\n";

        // Pilih karakter
        cout << "\n==================== Pilih Karakter Anda ====================\n" << endl;
        cout << left << setw(5) << "No" 
            << setw(20) << "Nama"
            << setw(10) << "HP"
            << setw(10) << "ATK"
            << setw(10) << "DEF" << endl;
        cout << "------------------------------------------------------------\n";

        for (int i = 0; i < dataUserSekarang.characters.size(); i++) {
            const UserCharacter& c = dataUserSekarang.characters[i];
            cout << left << setw(5) << i+1
                << setw(20) << c.nama
                << setw(10) << c.health
                << setw(10) << c.attack
                << setw(10) << c.defense << endl;
        }
        cout << "------------------------------------------------------------\n";
        cout << "0. Kembali" << endl;
        cout << "Pilihan: ";

        int charpilih;
        string input;
        getline(cin, input);
        
        if (!ScanInput(input, charpilih)) {
            throw runtime_error("Input tidak valid!");
        }

        if (charpilih == 0) return;
        if (charpilih < 1 || charpilih > dataUserSekarang.characters.size()) {
            throw runtime_error("Pilihan karakter tidak valid!");
        }

        UserCharacter player = dataUserSekarang.characters[charpilih-1];
        UserCharacter originalPlayer = player; // Simpan stat awal
        
        // Battle loop
        int turn = 1;
        bool playerRan = false;
        
        while (player.health > 0 && enemy.health > 0) {
            tampilkanStatusBattle(player, enemy, turn);
            
            // Player turn
            cout << "1. Serang\n";
            cout << "2. Kabur\n";
            cout << "Pilihan: ";
            
            int battlepilih;
            getline(cin, input);
            if (!ScanInput(input, battlepilih)) {
                cout << "Input tidak valid!\n";
                bersihkanBuffer();
                continue;
            }
            
            if (battlepilih == 1) {
                // Hitung damage player ke enemy
                int damage = max(1, player.attack - enemy.defense/2);
                enemy.health -= damage;
                cout << "\n" << player.nama << " menyerang " << enemy.nama << " dengan damage " << damage << "!\n";
                
                if (enemy.health <= 0) {
                    cout << enemy.nama << " dikalahkan!\n";
                    break;
                }
            } 
            else if (battlepilih == 2) {
                // 70% chance untuk kabur berhasil
                if (rand() % 10 < 7) {
                    cout << "Anda berhasil kabur dari pertarungan!\n";
                    playerRan = true;
                    break;
                } else {
                    cout << "Gagal kabur!\n";
                }
            }
            else {
                cout << "Pilihan tidak valid!\n";
                bersihkanBuffer();
                continue;
            }
            
            // Enemy turn (jika masih hidup)
            if (enemy.health > 0) {
                int damage = max(1, enemy.attack - player.defense/2);
                player.health -= damage;
                cout << enemy.nama << " menyerang " << player.nama << " dengan damage " << damage << "!\n";
                
                if (player.health <= 0) {
                    cout << player.nama << " kalah dalam pertarungan!\n";
                    break;
                }
            }

            bersihkanBuffer();
            
            turn++;
        }
        
        // Update karakter player
        dataUserSekarang.characters[charpilih-1] = player;
        
        // Jika menang, dapatkan koin
        if (enemy.health <= 0 && !playerRan) {
            int coinsEarned = (enemy.attack + enemy.defense) / 2;
            dataUserSekarang.coins += coinsEarned;
            cout << "\nAnda memenangkan pertarungan! Mendapatkan " << coinsEarned << " koin.\n";
            cout << "Total koin Anda sekarang: " << dataUserSekarang.coins << endl;
        }
        
        // Simpan data user
        for (auto& user : dataUser) {
            if (user.username == userSekarang) {
                user = dataUserSekarang;
                break;
            }
        }
        dataUserSekarang.characters[charpilih-1].health = originalPlayer.health;
        simpanUserData();
        
    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
    }
    enter();
}

// ===================== GACHA SYSTEM =====================
void gachaCharacter() {
    try {
        if (dataUserSekarang.coins < 50) {
            throw runtime_error("Koin tidak cukup! Diperlukan 50 koin untuk 1x spin.");
        }
        system("cls");
        cout << "\n=== GACHA CHARACTER ===" << endl;
        cout << "1x spin membutuhkan 50 koin\n";
        cout << "Koin Anda: " << dataUserSekarang.coins << endl;
        cout << "1. Spin (50 koin)\n";
        cout << "2. Kembali\n";
        cout << "Pilihan: ";

        int pilih;
        string input;
        getline(cin, input);
        if (!ScanInput(input, pilih)) {
            throw runtime_error("Input tidak valid!");
        }

        if (pilih == 2) return;
        if (pilih != 1) {
            throw runtime_error("Pilihan tidak valid!");
        }

        // Kurangi koin
        dataUserSekarang.coins -= 50;

        // 60% chance dapat karakter, 40% zonk
        if (rand() % 100 < 60 && jumlahKarakter > 0) {
            // Dapat karakter acak dari pool karakter
            int randomCharindeks = rand() % jumlahKarakter;
            Karakter charDapat = karakter[randomCharindeks];

            // Konversi ke UserCharacter
            UserCharacter newChar;
            newChar.nama = charDapat.nama;
            newChar.health = charDapat.health;
            newChar.attack = charDapat.attack;
            newChar.defense = charDapat.defense;

            // Tambahkan ke inventory user
            // Cek apakah karakter sudah dimiliki
            bool duplikat = false;
            for (const auto& owned : dataUserSekarang.characters) {
                if (owned.nama == newChar.nama) {
                    duplikat = true;
                    break;
                }
            }

            if (duplikat) {
                cout << "\nAnda mendapatkan karakter yang sudah dimiliki: " << newChar.nama << endl;
                cout << "Koin Anda dikembalikan sebagian: +10 koin (refund)\n";
                dataUserSekarang.coins += 10;
            } else {
                // Tambahkan ke inventory user
                dataUserSekarang.characters.push_back(newChar);

                cout << "\n==================================\n";
                cout << " SELAMAT! Anda mendapatkan:\n";
                cout << " " << newChar.nama << "\n";
                cout << " Health: " << newChar.health << "\n";
                cout << " Attack: " << newChar.attack << "\n";
                cout << " Defense: " << newChar.defense << "\n";
                cout << "==================================\n";
            }

        } else {
            cout << "\nMaaf, Anda tidak mendapatkan karakter apapun!\n";
            cout << "Coba lagi lain kali!\n";
        }

        // Update data user
        for (auto& user : dataUser) {
            if (user.username == userSekarang) {
                user = dataUserSekarang;
                break;
            }
        }
        simpanUserData();

    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
    }
    enter();
}

// ===================== MENU ADMIN  =====================
void adminMenu() {
    int pilihan;
    string input;
    
    while (true) {
        system("cls");
        cout << "\n=== MENU ADMIN (" << userSekarang << ") ===" << endl;
        cout << "1. Kelola Karakter" << endl;
        cout << "2. Kelola Musuh" << endl;
        cout << "3. Kelola User" << endl;  // New option
        cout << "4. Menu Pencarian" << endl;
        cout << "5. Logout" << endl;
        cout << "Pilihan: ";
        getline(cin, input);

        if (!ScanInput(input, pilihan)) {
            cout << "Input tidak valid! Harap masukkan angka 1-5." << endl;
            enter();
            continue;
        }

        switch (pilihan) {
            case 1: kelolaKarakter(); break;
            case 2: kelolaMusuh(); break;
            case 3: kelolaUser(); break;  
            case 4: menuSearch(); break;
            case 5: 
                is_login = false;
                is_admin = false;
                cout << "Logout berhasil." << endl;
                return;
            default:
                cout << "Pilihan tidak valid!" << endl;
        }
    }
}

void userMenu(bool adminsmenu = true) {
    int pilihan;
    string input;
    
    // Load user data saat pertama masuk menu
    static bool firstTime = true;
    if (firstTime) {
        muatUserData();
        loaddataUserSekarang();
        firstTime = false;
    }
    
    while (true) {
        
        if (adminsmenu) {
            system("cls");
            cout << "\n=== MENU USER (" << userSekarang << ") ===" << endl;
            cout << "Koin: " << dataUserSekarang.coins << endl;
            cout << "Jumlah Karakter: " << dataUserSekarang.characters.size() << endl;
        }
        cout << "1. Lihat Karakter" << endl;
        cout << "2. Lihat Karakter (Detailed)" << endl;
        cout << "3. Battle dengan Musuh" << endl;
        cout << "4. Gacha Character (50 koin)" << endl;
        cout << "5. Logout" << endl;
        cout << "Pilihan: ";
        getline(cin, input);

        if (!ScanInput(input, pilihan)) {
            cout << "Input tidak valid! Masukkan angka antara 1-5." << endl;
            continue;
        }

        switch (pilihan) {
            case 1: 
                tampilkanKarakter(); 
                break;
            case 2:
                tampilkanKarakter(true); 
                break;
            case 3:
                battle();
                break;
            case 4:
                gachaCharacter();
                break;
            case 5:
                is_login = false;
                firstTime = true; // Reset untuk login berikutnya
                cout << "Logout berhasil." << endl;
                return;
            default:
                cout << "Pilihan tidak valid!" << endl;
        }
    }
}

// ===================== MAIN PROGRAM ===================== 
int main(int argc, char* argv[]) {
    muatKarakterDariCSV();
    muatMusuhDariCSV();
    muatUserData();
    users[jumlahUser++] = {"demonia", "utakatik", true};  // User admin default

    int menu_utama;
    string input;

    while (true) {
        system("cls");
        if (!is_login) {
            cout << "=== MENU UTAMA ===" << endl;
            cout << "1. Login" << endl;
            cout << "2. Registrasi" << endl;
            cout << "3. Keluar" << endl;
            cout << "Pilihan: ";
            getline(cin, input);
            // cin >> menu_utama;

            if (!ScanInput(input, menu_utama)) {
                cout << "Input tidak valid! Masukkan angka." << endl;
                continue;
            }

            switch (menu_utama) {
                case 1: login(); break;
                case 2: registrasi (); break;
                case 3:    
                    cout << "Keluar dari program." << endl;
                        return 0;
                default:
                    cout << "Pilihan tidak valid!" << endl;
            }
        }
        else {
            if (is_admin) {
                adminMenu();
            }else {
                userMenu();
            }
        }
    }

    return 0;
}