#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <vector> 
#include <limits> 
#include <cstdlib>
#include <ctime>
#include <sstream>
using std::vector;  
using namespace std;


// ===================== STRUCT ===================== 

struct UserKarakter { //gacha
    string username;
    string nama;
    int health;
    int defense;
    int attack;
    string gambar;
};

struct Karakter {
    string nama;
    int health;
    int defense;
    int attack;
    string gambar;
};

struct Enemy {
    string nama;
    int health;
    int defense;
    int attack;
    string gambar;
};

struct User { //untuk menyimpan data user
    string username;
    string password;
    bool is_admin;
};

// Const
const int maks_karakter = 10;
const int maks_musuh = 10;
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

void enter(bool tampilkan_enter = true) {
    if (tampilkan_enter) {
        cout << "\nTekan Enter untuk kembali ke menu...";
    }
    cin.ignore(1000, '\n');
    cin.get();
}

void bersihkanBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

bool ScanInput(const string& input, int& output) {
    stringstream ss(input);
    ss >> output;

    // Cek apakah input valid (ada angka dan tidak gagal)
    if (ss.fail()) {
        return false;
    }

    return true;
}
// ===================== CSV FUNCTION ===================== 
// Fungsi untuk menyimpan data ke CSV

// Fungsi untuk menyimpan data user ke CSV
void simpanUserKeCSV() {
    ofstream file("users.csv");
    
    if (file.is_open()) {
        file << "Username,Password,IsAdmin\n";
        
        for (int i = 0; i < jumlahUser; i++) {
            file << users[i].username << ","
                 << users[i].password << ","
                 << users[i].is_admin << "\n";
        }
        file.close();
    }
}

// Fungsi untuk memuat data user dari CSV
void muatUserDariCSV() {
    ifstream file("users.csv");
    
    if (file.is_open()) {
        string line;
        getline(file, line); 
        
        jumlahUser = 0; 
        
        while (getline(file, line) && jumlahUser < maks_user) {
            stringstream ss(line);
            string cell;
            User newUser;
            
            getline(ss, newUser.username, ',');
            getline(ss, newUser.password, ',');
            
            string temp;
            getline(ss, temp, ',');
            newUser.is_admin = (temp == "1");
            
            users[jumlahUser++] = newUser;
        }
        file.close();
    }
    
    // Tambahkan admin default jika tidak ada
    bool adminExists = false;
    for (int i = 0; i < jumlahUser; i++) {
        if (users[i].username == "demonia" && users[i].is_admin) {
            adminExists = true;
            break;
        }
    }
    
    if (!adminExists && jumlahUser < maks_user) {
        users[jumlahUser++] = {"demonia", "utakatik", true};
        simpanUserKeCSV();
    }
}

// Fungsi untuk memuat karakter user dari CSV
void muatKarakterUser(vector<UserKarakter>& karakterUser) {
    ifstream file("characteruser.csv");
    
    if (file.is_open()) {
        string line;
        getline(file, line); // Skip header
        
        while (getline(file, line)) {
            stringstream ss(line);
            string cell;
            UserKarakter uk;
            
            getline(ss, uk.username, ',');
            getline(ss, uk.nama, ',');
            
            string temp;
            getline(ss, temp, ',');
            uk.attack = stoi(temp);
            
            getline(ss, temp, ',');
            uk.health = stoi(temp);
            
            getline(ss, temp, ',');
            uk.defense = stoi(temp);
            
            getline(ss, uk.gambar, ',');
            
            karakterUser.push_back(uk);
        }
        file.close();
    }
}

// Fungsi untuk menyimpan karakter user ke CSV
void simpanKarakterUser(const vector<UserKarakter>& karakterUser) {
    ofstream file("characteruser.csv");
    
    if (file.is_open()) {
        file << "Username,Nama,Attack,Health,Defense,Gambar\n";
        
        for (const auto& uk : karakterUser) {
            file << uk.username << ","
                 << uk.nama << ","
                 << uk.attack << ","
                 << uk.health << ","
                 << uk.defense << ","
                 << uk.gambar << "\n";
        }
        file.close();
    }
}

void simpanKeCSV() {
    ofstream file("characters.csv");
    try {
        // Cek apakah file bisa dibuka
        if (!file.is_open()) {
            throw runtime_error("Gagal membuka file characters.csv untuk ditulis!");
        }
        
        // Header CSV
        file << "Nama,Attack,Health,Defense,Gambar\n";
        
        // Tulis data
        for (int i = 0; i < jumlahKarakter; i++) {
            file << karakter[i].nama << ","
                 << karakter[i].attack << ","
                 << karakter[i].health << ","
                 << karakter[i].defense << ","
                 << karakter[i].gambar << "\n";
        }

        // Cek apakah penulisan berhasil
        if (file.fail()) {
            throw runtime_error("Gagal menulis data ke file!");
        }

        cout << "Data karakter berhasil disimpan ke characters.csv\n";
    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
    }
    file.close(); 
}

void simpanMusuhKeCSV() {
    ofstream file("enemy.csv");
    try {
        if (!file.is_open()) {
            throw runtime_error("Gagal membuka file enemy.csv untuk ditulis!");
        }

        file << "Nama,Attack,Health,Defense,Gambar\n";
        
        for (int i = 0; i < jumlahMusuh; i++) {
            file << musuh[i].nama << ","
                 << musuh[i].attack << ","
                 << musuh[i].health << ","
                 << musuh[i].defense << ","
                 << musuh[i].gambar << "\n";

            if (file.fail()) {
                throw runtime_error("Gagal menulis data musuh ke file!");
            }
        }
        cout << "Data musuh berhasil disimpan ke enemy.csv (" 
             << jumlahMusuh << " data)\n";
    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
    }
    if (file.is_open()) {
        file.close();
    }
}

void muatKarakterDariCSV() {
    ifstream file("characters.csv");
    try {
        // Cek apakah file ada
        if (!file.is_open()) {
            throw runtime_error("File characters.csv tidak ditemukan!");
        }

        string line;
        getline(file, line); 

        jumlahKarakter = 0;

        while (getline(file, line)) {
            if (line.empty()) continue; 

            vector<string> row;
            stringstream ss(line);
            string cell;

            while (getline(ss, cell, ',')) {
                row.push_back(cell);
            }

            if (row.size() != 5) {
                throw runtime_error("Format file CSV tidak valid (kolom tidak sesuai)!");
            }

            if (jumlahKarakter >= maks_karakter) {
                throw runtime_error("Kapasitas karakter penuh, tidak bisa memuat lebih banyak data.");
            }

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

            karakter[jumlahKarakter].gambar = row[4];
            jumlahKarakter++;
        }

        cout << "Data karakter berhasil dimuat (" << jumlahKarakter << " entri)\n";
    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
        jumlahKarakter = 0; 
    }
    file.close();
}

void muatMusuhDariCSV() {
    ifstream file("enemy.csv");
    try {
        if (!file.is_open()) {
            throw runtime_error("File enemy.csv tidak ditemukan!");
        }

        string line;
        getline(file, line); 
        jumlahMusuh = 0;

        while (getline(file, line)) {
            if (line.empty()) continue;

            vector<string> row;
            stringstream ss(line);
            string cell;

            while (getline(ss, cell, ',')) {
                row.push_back(cell);
            }

            if (row.size() != 5) {
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

            musuh[jumlahMusuh].gambar = row[4];
            jumlahMusuh++;
        }

        cout << "Data musuh berhasil dimuat (" << jumlahMusuh << " entri)\n";
    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
        jumlahMusuh = 0;
    }
    file.close();
}

// ===================== MUSUH FUNCTIONS ===================== 
void tambahMusuh() {
    try {
        if (jumlahMusuh >= maks_musuh) {
            throw runtime_error("Data musuh penuh! Maksimal: " + to_string(maks_musuh));
        }

        Enemy newEnemy;
        cout << "Nama Musuh: ";
        cin >> newEnemy.nama;

        // Cek duplikasi nama
        for (int i = 0; i < jumlahMusuh; i++) {
            if (musuh[i].nama == newEnemy.nama) {
                throw runtime_error("Nama musuh sudah digunakan!");
            }
        }

        cout << "Attack: ";
        if (!(cin >> newEnemy.attack) || newEnemy.attack < 0) {
            throw runtime_error("Attack harus angka >= 0!");
        }

        // Validasi health dan defense
        cout << "Health: ";
        if (!(cin >> newEnemy.health) || newEnemy.health < 0) {
            throw runtime_error("Health harus angka >= 0!");
        }

        cout << "Defense: ";
        if (!(cin >> newEnemy.defense) || newEnemy.defense < 0) {
            throw runtime_error("Defense harus angka >= 0!");
        }

        cout << "Link Gambar: ";
        cin >> newEnemy.gambar;
        bersihkanBuffer();

        musuh[jumlahMusuh++] = newEnemy;
        simpanMusuhKeCSV();
        cout << "Musuh berhasil ditambahkan!\n";

    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

void tampilkanMusuh() {
    if (jumlahMusuh == 0) {
        cout << "Tidak ada musuh yang tersimpan.\n";
        enter();
        return;
    }

    cout << "\n==================== Daftar Musuh ====================\n"<< endl;
    cout << left << setw(5) << "No" 
         << setw(15) << "Nama"
         << setw(10) << "Attack"
         << setw(10) << "Health"
         << setw(10) << "Defense" << endl;
    cout << "------------------------------------------------------------\n";

    for (int i = 0; i < jumlahMusuh; i++) {
        cout << left << setw(5) << i+1
             << setw(15) << musuh[i].nama
             << setw(10) << musuh[i].attack 
             << setw(10) << musuh[i].health
             << setw(10) << musuh[i].defense << endl;
    }
    cout << "------------------------------------------------------------\n";

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
            cout << "Input tidak valid! Harap masukkan angka 1-3." << endl;
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
                cout << "\nMusuh setelah diurutkan (Attack Descending):\n";
            }
            
            // Tampilkan hasil sorting
            cout << "------------------------------------------------------------\n";
            for (int i = 0; i < jumlahMusuh; i++) {
                cout << left << setw(5) << i+1
                     << setw(15) << sortedMusuh[i].nama
                     << setw(10) << sortedMusuh[i].attack 
                     << setw(10) << sortedMusuh[i].health
                     << setw(10) << sortedMusuh[i].defense << endl;
            }
            cout << "------------------------------------------------------------\n";
        } else if (pilihan != 3) {
            cout << "Pilihan tidak valid! Harap masukkan angka 1-3." << endl;
        }
    } while (pilihan != 3);
}

void ubahMusuh(int startIndex = 0) {
    try {
        if (jumlahMusuh == 0) {
            cout << "Tidak ada karakter yang tersimpan.\n";
            enter();
            return;
        } 

        // Tampilkan daftar karakter
        tampilkanMusuh();

        int index;
        cout << "Masukkan nomor musuh yang ingin diubah: ";
        if (!(cin >> index) || index < startIndex + 1 || index > jumlahMusuh) {
            throw runtime_error("Nomor tidak valid!");
        }
        index--;

        // Input data baru
        string namaBaru;
        cout << "Nama Baru: ";
        cin >> namaBaru;

        // Cek duplikasi nama (kecuali untuk dirinya sendiri)
        for (int i = 0; i < jumlahMusuh; i++) {
            if (i != index && musuh[i].nama == namaBaru) {
                throw runtime_error("Nama sudah digunakan oleh musuh lain!");
            }
        }

        musuh[index].nama = namaBaru;

        cout << "Attack Baru: ";
        if (!(cin >> musuh[index].attack) || musuh[index].attack < 0) {
            throw runtime_error("Attack harus angka >= 0!");
        }

        // Validasi health dan defense
        cout << "Health Baru: ";
        if (!(cin >> musuh[index].health) || musuh[index].health < 0) {
            throw runtime_error("Health harus angka >= 0!");
        }

        cout << "Defense Baru: ";
        if (!(cin >> musuh[index].defense) || musuh[index].defense < 0) {
            throw runtime_error("Defense harus angka >= 0!");
        }

        cout << "Link Gambar Baru: ";
        cin >> musuh[index].gambar;

        simpanMusuhKeCSV();
        cout << "Musuh berhasil diubah!\n";
        bersihkanBuffer();


    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

void hapusMusuh(bool confirm = false, int index = -1) {
    if (jumlahMusuh == 0) {
        cout << "Tidak ada musuh yang tersimpan.\n";
        enter();
        return;
    } 
    
    try {
        if (!confirm) {
            tampilkanMusuh();
            
            cout << "Masukkan nomor musuh yang ingin dihapus: ";
            if (!(cin >> index) || index < 1 || index > jumlahMusuh) {
                throw runtime_error("Nomor tidak valid!");
            }
            index--;

            cout << "Apakah Anda yakin ingin menghapus " << musuh[index].nama << "? (y/n): ";
            char choice;
            cin >> choice;
            bersihkanBuffer();

            if (choice == 'y' || choice == 'Y') {
                hapusMusuh(true, index);
            } else {
                cout << "Penghapusan dibatalkan.\n";
                return;
            }
        } else {
            // Proses penghapusan
            for (int i = index; i < jumlahMusuh - 1; i++) {
                musuh[i] = musuh[i + 1];
            }
            jumlahMusuh--;
            simpanMusuhKeCSV();
            cout << "Musuh berhasil dihapus!\n";
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
    cout << "\nDetail Karakter:" << endl;
    cout << "Nama: " << karakterPtr->nama << endl;
    cout << "Attack: " << karakterPtr->attack << endl;
    cout << "Health: " << karakterPtr->health << endl;
    cout << "Defense: " << karakterPtr->defense << endl;
    cout << "Gambar: " << karakterPtr->gambar << endl;
}

// Fungsi dengan parameter address-of
void updateHealth(Karakter &karakterRef, int newHealth) {
    try {
        if (newHealth < 0) throw runtime_error("Health tidak boleh negatif!");
        karakterRef.health = newHealth;
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
        int ukuranKiri = tengah - kiri + 1;
        int ukuranKanan = kanan - tengah;

        // Buat array sementara
        Karakter arrayKiri[ukuranKiri], arrayKanan[ukuranKanan];

        // Salin data ke array sementara
        for (int i = 0; i < ukuranKiri; i++)
            arrayKiri[i] = karakter[kiri + i];
        for (int j = 0; j < ukuranKanan; j++)
            arrayKanan[j] = karakter[tengah + 1 + j];

        // Gabungkan kembali
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
        // Base case: array dengan 1 elemen sudah terurut
        if (kiri >= kanan) {
            return;
        }

        int tengah = kiri + (kanan - kiri) / 2;
        
        // Rekursif: urutkan bagian kiri dan kanan
        mergeSortKarakter(karakter, kiri, tengah);
        mergeSortKarakter(karakter, tengah + 1, kanan);
        
        // Gabungkan kedua bagian yang sudah terurut
        merge(karakter, kiri, tengah, kanan);

    } catch (const exception& e) {
        cerr << "[ERROR] MergeSort gagal: " << e.what() << endl;
    }
}


// ===================== MAIN MENU FUNCTION ===================== 
void login(int maks_percobaan = maks_attempt) {
    int attempt = 0;
    while (attempt < maks_percobaan) {
        try {
            string username, password;
            cout << "\n=== LOGIN ===" << endl;
            cout << "Percobaan ke-" << attempt + 1 << " dari " << maks_percobaan << endl;
            
            cout << "Username: ";
            if (!(cin >> username)) throw runtime_error("Input username tidak valid!");
            
            cout << "Password: ";
            if (!(cin >> password)) throw runtime_error("Input password tidak valid!");

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            // Cek admin khusus
            if (username == "demonia" && password == "utakatik") {
                is_login = true;
                userSekarang = username;
                is_admin = true;
                cout << "Login sebagai admin berhasil!\n";
                return;
            }

            // Cek dari data user
            bool found = false;
            for (int i = 0; i < jumlahUser; i++) {
                if (users[i].username == username && users[i].password == password) {
                    found = true;
                    is_login = true;
                    userSekarang = username;
                    is_admin = users[i].is_admin;
                    cout << "Login berhasil!\n";
                    return;
                }
            }
            if (!found) throw runtime_error("Username/password salah!");
            
        } catch (const exception& e) {
            cerr << "[ERROR] " << e.what() << endl;
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

        for (int i = 0; i < jumlahUser; i++) {
            if (users[i].username == username) {
                throw runtime_error("Username sudah digunakan!");
            }
        }

        cout << "Password: ";
        if (!(cin >> password) || password.empty()) {
            throw runtime_error("Password tidak boleh kosong!");
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        users[jumlahUser].username = username;
        users[jumlahUser].password = password;
        users[jumlahUser].is_admin = false; // Default: user biasa
        jumlahUser++;

        // Simpan ke CSV
        simpanUserKeCSV();

        cout << "Registrasi berhasil! Silakan login.\n";

    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
        cin.clear(); 
        cin.ignore(1000, '\n'); 
    }
}

// ===================== CHARACTER FUNCTION ===================== 
void tambahKarakter(int maxCharacters = maks_karakter) {
    try {
        if (jumlahKarakter >= maxCharacters) {
            throw runtime_error("Data karakter penuh! Maksimal: " + to_string(maxCharacters));
        }

        Karakter newKarakter;
        cout << "Nama Karakter: ";
        cin >> newKarakter.nama;

        // Cek duplikasi nama
        for (int i = 0; i < jumlahKarakter; i++) {
            if (karakter[i].nama == newKarakter.nama) {
                throw runtime_error("Nama karakter sudah digunakan!");
            }
        }

        cout << "Attack: ";
        if (!(cin >> newKarakter.attack) || newKarakter.attack < 0) {
            throw runtime_error("Attack harus angka >= 0!");
        }

        cout << "Health: ";
        if (!(cin >> newKarakter.health) || newKarakter.health < 0) {
            throw runtime_error("Health harus angka >= 0!");
        }

        cout << "Defense: ";
        if (!(cin >> newKarakter.defense) || newKarakter.defense < 0) {
            throw runtime_error("Defense harus angka >= 0!");
        }

        cout << "Link Gambar: ";
        cin >> newKarakter.gambar;
        bersihkanBuffer();

        karakter[jumlahKarakter++] = newKarakter;
        simpanKeCSV();
        cout << "Karakter berhasil ditambahkan!\n";

    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

void tampilkanKarakterCRUD(){
    cout << "\n==================== Daftar Karakter ====================\n"<< endl;
        cout << left << setw(5) << "No" 
             << setw(15) << "Nama"
             << setw(10) << "Attack"
             << setw(10) << "Health"
             << setw(10) << "Defense" << endl;
        cout << "------------------------------------------------------------\n";

        for (int i = 0; i < jumlahKarakter; i++) {
            cout << left << setw(5) << i+1
                 << setw(15) << karakter[i].nama
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
        cout << "\n==================== Daftar Karakter (Detailed) ====================\n"<< endl;
        cout << left << setw(5) << "No" 
             << setw(20) << "Nama"
             << setw(15) << "Attack"
             << setw(15) << "Health"
             << setw(15) << "Defense"
             << setw(25) << "Gambar" << endl;
        cout << "----------------------------------------------------------------------------\n";

        for (int i = 0; i < jumlahKarakter; i++) {
            cout << left << setw(5) << i+1
                 << setw(20) << karakter[i].nama
                 << setw(15) << karakter[i].attack 
                 << setw(15) << karakter[i].health
                 << setw(15) << karakter[i].defense
                 << setw(25) << karakter[i].gambar << endl;
        }
        cout << "----------------------------------------------------------------------------\n";
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
        cout << "Input tidak valid! Masukkan angka antara 1-3." << endl;
        continue;
    }

    if (pilihan >= 1 && pilihan <= 2) {
        Karakter sortedKarakter[maks_karakter];
        salinKarakter(sortedKarakter, karakter, jumlahKarakter);
        
        if (pilihan == 1) {
            bubbleSortKarakter(sortedKarakter, jumlahKarakter);
            cout << "\nKarakter setelah diurutkan (Nama Ascending):\n";
        } else if (pilihan == 2) {
            mergeSortKarakter(sortedKarakter, 0, jumlahKarakter-1);
            cout << "\nKarakter setelah diurutkan (Attack Descending):\n";
        }
        
        // Tampilkan hasil sorting
        cout << "\n==================== Daftar Karakter (Sorted) ====================\n"<< endl;
        cout << left << setw(5) << "No" 
             << setw(15) << "Nama"
             << setw(10) << "Attack"
             << setw(10) << "Health"
             << setw(10) << "Defense" << endl;
        cout << "------------------------------------------------------------\n";

        for (int i = 0; i < jumlahKarakter; i++) {
            cout << left << setw(5) << i+1
                 << setw(15) << sortedKarakter[i].nama
                 << setw(10) << sortedKarakter[i].attack 
                 << setw(10) << sortedKarakter[i].health
                 << setw(10) << sortedKarakter[i].defense << endl;
        }
        cout << "------------------------------------------------------------\n";
    } else if (pilihan != 3) {
        cout << "Pilihan tidak valid! Masukkan angka antara 1-3." << endl;
    }
} while (pilihan != 3);
}

void ubahKarakter(int startIndex = 0) {
    try {
        if (jumlahKarakter == 0) {
            cout << "Tidak ada karakter yang tersimpan.\n";
            enter();
            return;
        } 

        // Tampilkan daftar karakter
        tampilkanKarakterCRUD();

        int index;
        cout << "Masukkan nomor karakter yang ingin diubah: ";
        if (!(cin >> index) || index < startIndex + 1 || index > jumlahKarakter) {
            throw runtime_error("Nomor tidak valid! ");
        }
        index--;

        // Input data baru
        string namaBaru;
        cout << "Nama Baru: ";
        cin >> namaBaru;

        // Cek duplikasi nama (kecuali untuk dirinya sendiri)
        for (int i = 0; i < jumlahKarakter; i++) {
            if (i != index && karakter[i].nama == namaBaru) {
                throw runtime_error("Nama sudah digunakan oleh karakter lain!");
            }
        }

        karakter[index].nama = namaBaru;

        cout << "Attack Baru: ";
        if (!(cin >> karakter[index].attack) || karakter[index].attack < 0) {
            throw runtime_error("Attack harus angka >= 0!");
        }

        // Validasi health dan defense
        cout << "Health Baru: ";
        if (!(cin >> karakter[index].health) || karakter[index].health < 0) {
            throw runtime_error("Health harus angka >= 0!");
        }

        cout << "Defense Baru: ";
        if (!(cin >> karakter[index].defense) || karakter[index].defense < 0) {
            throw runtime_error("Defense harus angka >= 0!");
        }

        cout << "Link Gambar Baru: ";
        cin >> karakter[index].gambar;
        bersihkanBuffer();

        simpanKeCSV();
        cout << "Karakter berhasil diubah!\n";

    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

void hapusKarakter(bool confirm = false, int index = -1) {
    if (jumlahKarakter == 0) {
        cout << "Tidak ada karakter yang tersimpan.\n";
        enter();
        return;
    } 
    
    try {
        if (!confirm) {
            tampilkanKarakterCRUD();
            
            cout << "Masukkan nomor karakter yang ingin dihapus: ";
            if (!(cin >> index) || index < 1 || index > jumlahKarakter) {
                throw runtime_error("Nomor tidak valid!");
            }
            index--;

            cout << "Apakah Anda yakin ingin menghapus " << karakter[index].nama << "? (y/n): ";
            char choice;
            cin >> choice;
            bersihkanBuffer();

            if (choice == 'y' || choice == 'Y') {
                hapusKarakter(true, index);
            } else {
                cout << "Penghapusan dibatalkan.\n";
                return;
            }
        } else {
            // Proses penghapusan
            for (int i = index; i < jumlahKarakter - 1; i++) {
                karakter[i] = karakter[i + 1];
            }
            jumlahKarakter--;
            simpanKeCSV();
            cout << "Karakter berhasil dihapus!\n";
        }
    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

// Fungsi untuk gacha karakter
void gachaKarakter() {
    if (jumlahKarakter == 0) {
        cout << "Tidak ada karakter yang tersedia untuk gacha.\n";
        return;
    }

    // Ambil karakter acak
    srand(time(0));
    int randomIndex = rand() % jumlahKarakter;
    Karakter karakterTerpilih = karakter[randomIndex];

    // Simpan ke karakter user
    vector<UserKarakter> karakterUser;
    muatKarakterUser(karakterUser);
    
    UserKarakter uk;
    uk.username = userSekarang;
    uk.nama = karakterTerpilih.nama;
    uk.attack = karakterTerpilih.attack;
    uk.health = karakterTerpilih.health;
    uk.defense = karakterTerpilih.defense;
    uk.gambar = karakterTerpilih.gambar;
    
    karakterUser.push_back(uk);
    simpanKarakterUser(karakterUser);

    cout << "\n========================================\n";
    cout << " SELAMAT! Anda mendapatkan karakter:\n";
    cout << " Nama: " << karakterTerpilih.nama << endl;
    cout << " Attack: " << karakterTerpilih.attack << endl;
    cout << " Health: " << karakterTerpilih.health << endl;
    cout << " Defense: " << karakterTerpilih.defense << endl;
    cout << "========================================\n";
}

// Fungsi untuk menampilkan karakter user
void tampilkanKarakterUser() {
    vector<UserKarakter> karakterUser;
    muatKarakterUser(karakterUser);

    int count = 0;

    cout << "\n=== KARAKTER MILIK " << userSekarang << " ===\n";
    cout << "------------------------------------------------------------\n";
    cout << left << setw(5) << "No" << setw(15) << "Nama" 
         << setw(10) << "Attack" << setw(10) << "Health" << setw(10) << "Defense" << endl;
    cout << "------------------------------------------------------------\n";

    for (const auto& uk : karakterUser) {
        if (uk.username == userSekarang) {
            cout << left << setw(5) << ++count
                 << setw(15) << uk.nama
                 << setw(10) << uk.attack
                 << setw(10) << uk.health
                 << setw(10) << uk.defense << endl;
        }
    }

    if (count == 0) {
        cout << "Anda belum memiliki karakter.\n";
    }
    cout << "------------------------------------------------------------\n";
}
// ===================== ERROR HANDLING ===================== 


//========================== SEARCHING ==========================
// Linear Search untuk Karakter (berdasarkan nama)
void linearSearchKarakter(const string& nama) {
    try {
        if (jumlahKarakter == 0) throw runtime_error("Tidak ada karakter yang tersimpan!");
        
        bool found = false;
        cout << "\n=== HASIL PENCARIAN KARAKTER ===" << endl;
        cout << "------------------------------------------------------------" << endl;
        cout << left << setw(15) << "Nama" << setw(10) << "Attack" 
             << setw(10) << "Health" << setw(10) << "Defense" << endl;
        cout << "------------------------------------------------------------" << endl;
        
        for (int i = 0; i < jumlahKarakter; i++) {
            if (karakter[i].nama.find(nama) != string::npos) {
                cout << left << setw(15) << karakter[i].nama 
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
    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
    }
}

// Linear Search untuk Musuh (berdasarkan nama)
void linearSearchMusuh(const string& nama) {
    try {
        if (jumlahMusuh == 0) throw runtime_error("Tidak ada musuh yang tersimpan!");
        
        bool found = false;
        cout << "\n=== HASIL PENCARIAN MUSUH ===" << endl;
        cout << "------------------------------------------------------------" << endl;
        cout << left << setw(15) << "Nama" << setw(10) << "Attack" 
             << setw(10) << "Health" << setw(10) << "Defense" << endl;
        cout << "------------------------------------------------------------" << endl;
        
        for (int i = 0; i < jumlahMusuh; i++) {
            if (musuh[i].nama.find(nama) != string::npos) {
                cout << left << setw(15) << musuh[i].nama 
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
    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
    }
}

// Binary Search untuk Karakter (bisa untuk attack, health, atau defense)
void binarySearchKarakter(int searchValue, SearchType type) {
    try {
        if (jumlahKarakter == 0) throw runtime_error("Tidak ada karakter yang tersimpan!");
        
        // Urutkan dulu untuk binary search
        Karakter sortedKarakter[maks_karakter];
        salinKarakter(sortedKarakter, karakter, jumlahKarakter);
        mergeSortKarakter(sortedKarakter, 0, jumlahKarakter-1); // Urutkan descending
        
        int low = 0;
        int high = jumlahKarakter - 1;
        bool found = false;
        
        string searchField;
        switch(type) {
            case ATTACK: searchField = "Attack"; break;
            case HEALTH: searchField = "Health"; break;
            case DEFENSE: searchField = "Defense"; break;
        }
        
        cout << "\n=== HASIL PENCARIAN KARAKTER (Binary Search - " << searchField << ": " << searchValue << ") ===" << endl;
        cout << "------------------------------------------------------------" << endl;
        cout << left << setw(15) << "Nama" << setw(10) << "Attack" 
             << setw(10) << "Health" << setw(10) << "Defense" << endl;
        cout << "------------------------------------------------------------" << endl;
        
        while (low <= high) {
            int mid = low + (high - low) / 2;
            int currentValue;
            
            switch(type) {
                case ATTACK: currentValue = sortedKarakter[mid].attack; break;
                case HEALTH: currentValue = sortedKarakter[mid].health; break;
                case DEFENSE: currentValue = sortedKarakter[mid].defense; break;
            }
            
            if (currentValue == searchValue) {
                // Tampilkan hanya yang nilainya sama persis
                cout << left << setw(15) << sortedKarakter[mid].nama 
                     << setw(10) << sortedKarakter[mid].attack
                     << setw(10) << sortedKarakter[mid].health
                     << setw(10) << sortedKarakter[mid].defense << endl;
                found = true;
                
                // Cari ke kiri untuk nilai yang sama
                int left = mid - 1;
                while (left >= 0) {
                    switch(type) {
                        case ATTACK: currentValue = sortedKarakter[left].attack; break;
                        case HEALTH: currentValue = sortedKarakter[left].health; break;
                        case DEFENSE: currentValue = sortedKarakter[left].defense; break;
                    }
                    if (currentValue == searchValue) {
                        cout << left << setw(15) << sortedKarakter[left].nama 
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
                        case ATTACK: currentValue = sortedKarakter[right].attack; break;
                        case HEALTH: currentValue = sortedKarakter[right].health; break;
                        case DEFENSE: currentValue = sortedKarakter[right].defense; break;
                    }
                    if (currentValue == searchValue) {
                        cout << left << setw(15) << sortedKarakter[right].nama 
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
            
            if (currentValue < searchValue) {
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }
        
        if (!found) {
            cout << "Tidak ditemukan karakter dengan " << searchField << " = " << searchValue << endl;
        }
        cout << "------------------------------------------------------------" << endl;
    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
    }
}

// Binary Search untuk Musuh (bisa untuk attack, health, atau defense)
void binarySearchMusuh(int searchValue, SearchType type) {
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
        
        string searchField;
        switch(type) {
            case ATTACK: searchField = "Attack"; break;
            case HEALTH: searchField = "Health"; break;
            case DEFENSE: searchField = "Defense"; break;
        }
        
        cout << "\n=== HASIL PENCARIAN MUSUH (Binary Search - " << searchField << ": " << searchValue << ") ===" << endl;
        cout << "------------------------------------------------------------" << endl;
        cout << left << setw(15) << "Nama" << setw(10) << "Attack" 
             << setw(10) << "Health" << setw(10) << "Defense" << endl;
        cout << "------------------------------------------------------------" << endl;
        
        while (low <= high) {
            int mid = low + (high - low) / 2;
            int currentValue;
            
            switch(type) {
                case ATTACK: currentValue = sortedMusuh[mid].attack; break;
                case HEALTH: currentValue = sortedMusuh[mid].health; break;
                case DEFENSE: currentValue = sortedMusuh[mid].defense; break;
            }
            
            if (currentValue == searchValue) {
                // Tampilkan semua yang sama
                int temp = mid;
                while (temp >= 0) {
                    switch(type) {
                        case ATTACK: if (sortedMusuh[temp].attack != searchValue) break; break;
                        case HEALTH: if (sortedMusuh[temp].health != searchValue) break; break;
                        case DEFENSE: if (sortedMusuh[temp].defense != searchValue) break; break;
                    }
                    cout << left << setw(15) << sortedMusuh[temp].nama 
                         << setw(10) << sortedMusuh[temp].attack
                         << setw(10) << sortedMusuh[temp].health
                         << setw(10) << sortedMusuh[temp].defense << endl;
                    temp--;
                    found = true;
                }
                temp = mid + 1;
                while (temp < jumlahMusuh) {
                    switch(type) {
                        case ATTACK: if (sortedMusuh[temp].attack != searchValue) break; break;
                        case HEALTH: if (sortedMusuh[temp].health != searchValue) break; break;
                        case DEFENSE: if (sortedMusuh[temp].defense != searchValue) break; break;
                    }
                    cout << left << setw(15) << sortedMusuh[temp].nama 
                         << setw(10) << sortedMusuh[temp].attack
                         << setw(10) << sortedMusuh[temp].health
                         << setw(10) << sortedMusuh[temp].defense << endl;
                    temp++;
                    found = true;
                }
                break;
            }
            
            if (currentValue < searchValue) {
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }
        
        if (!found) {
            cout << "Tidak ditemukan musuh dengan " << searchField << " = " << searchValue << endl;
        }
        cout << "------------------------------------------------------------" << endl;
    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
    }
}

void menuSearchKarakter() {
    int searchMethod;
    string input;
    cout << "\n=== PENCARIAN KARAKTER ===" << endl;
    cout << "1. Cari berdasarkan Nama (Linear Search)" << endl;
    cout << "2. Cari berdasarkan Attack (Binary Search)" << endl;
    cout << "3. Cari berdasarkan Health (Binary Search)" << endl;
    cout << "4. Cari berdasarkan Defense (Binary Search)" << endl;
    cout << "5. Kembali" << endl;
    cout << "Pilihan: ";
    getline(cin, input);

    if (!ScanInput(input, searchMethod)) {
        cout << "Input tidak valid! Masukkan angka antara 1-5." << endl;
        return;
    }
    
    if (searchMethod == 1) {
        string nama;
        cout << "Masukkan nama (atau bagian nama) yang dicari: ";
        getline(cin, nama);
        linearSearchKarakter(nama);
    } else if (searchMethod >= 2 && searchMethod <= 4) {
        int value;
        string field, valueInput;
        switch(searchMethod) {
            case 2: field = "attack"; break;
            case 3: field = "health"; break;
            case 4: field = "defense"; break;
        }
        cout << "Masukkan nilai " << field << " yang dicari: ";
        getline(cin, valueInput);

        if (!ScanInput(valueInput, value)) {
            cout << "Input tidak valid! Masukkan angka untuk " << field << "." << endl;
            return;
        }
        
        SearchType type;
        switch(searchMethod) {
            case 2: type = ATTACK; break;
            case 3: type = HEALTH; break;
            case 4: type = DEFENSE; break;
        }
        binarySearchKarakter(value, type);
    } else if (searchMethod != 5) {
        cout << "Pilihan tidak valid! Masukkan angka antara 1-5." << endl;
    }
}

void menuSearchMusuh() {
    int searchMethod;
    string input;
    cout << "\n=== PENCARIAN MUSUH ===" << endl;
    cout << "1. Cari berdasarkan Nama (Linear Search)" << endl;
    cout << "2. Cari berdasarkan Attack (Binary Search)" << endl;
    cout << "3. Cari berdasarkan Health (Binary Search)" << endl;
    cout << "4. Cari berdasarkan Defense (Binary Search)" << endl;
    cout << "5. Kembali" << endl;
    cout << "Pilihan: ";
    getline(cin, input);

    if (!ScanInput(input, searchMethod)) {
        cout << "Input tidak valid! Masukkan angka antara 1-5." << endl;
        return;
    }
    
    if (searchMethod == 1) {
        string nama;
        cout << "Masukkan nama (atau bagian nama) yang dicari: ";
        getline(cin, nama);
        linearSearchMusuh(nama);
    } else if (searchMethod >= 2 && searchMethod <= 4) {
        int value;
        string field, valueInput;
        switch(searchMethod) {
            case 2: field = "attack"; break;
            case 3: field = "health"; break;
            case 4: field = "defense"; break;
        }
        cout << "Masukkan nilai " << field << " yang dicari: ";
        getline(cin, valueInput);

        if (!ScanInput(valueInput, value)) {
            cout << "Input tidak valid! Masukkan angka untuk " << field << "." << endl;
            return;
        }
        
        SearchType type;
        switch(searchMethod) {
            case 2: type = ATTACK; break;
            case 3: type = HEALTH; break;
            case 4: type = DEFENSE; break;
        }
        binarySearchMusuh(value, type);
    } else if (searchMethod != 5) {
        cout << "Pilihan tidak valid! Masukkan angka antara 1-5." << endl;
    }
}

void menuSearch() {
    int searchChoice;
    string input;

    cout << "\n=== MENU PENCARIAN ===" << endl;
    cout << "1. Cari Karakter" << endl;
    cout << "2. Cari Musuh" << endl;
    cout << "3. Kembali" << endl;
    cout << "Pilihan: ";
    getline(cin, input);

    if (!ScanInput(input, searchChoice)) {
        cout << "Input tidak valid! Harap masukkan angka 1-3." << endl;
        enter();
        return;
    }
    
    switch(searchChoice) {
        case 1: 
            menuSearchKarakter();
            break;
        case 2:
            menuSearchMusuh();
            break;
        case 3:
            // Kembali ke menu sebelumnya
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
        cout << "\n=== KELOLA KARAKTER ===" << endl;
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
        cout << "\n=== KELOLA MUSUH ===" << endl;
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
            case 5: return; // Kembali ke menu admin
            default:
            cout << "Pilihan tidak valid!" << endl;
        }
    }
}

// ===================== SUB MENU FUNCTION =====================
void adminMenu() {
    int pilihan;
    string input;
    
    while (true) {
        cout << "\n=== MENU ADMIN (" << userSekarang << ") ===" << endl;
        cout << "1. Kelola Karakter" << endl;
        cout << "2. Kelola Musuh" << endl;
        cout << "3. Menu Pencarian" << endl;
        cout << "4. Logout" << endl;
        cout << "Pilihan: ";
        getline(cin, input);
        
        if (!ScanInput(input, pilihan)) {
            cout << "Input tidak valid! Harap masukkan angka 1-4." << endl;
            enter();
            continue;
        }
        
        switch (pilihan) {
            case 1: kelolaKarakter(); break;
            case 2: kelolaMusuh(); break;
            case 3: menuSearch(); break;
            case 4: 
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
    
    while (is_login) {  // Tambahkan loop agar menu tetap muncul
        if (adminsmenu) {
            cout << "\n=== MENU USER (" << userSekarang << ") ===" << endl;
        }
        cout << "1. Lihat Karakter" << endl;
        cout << "2. Lihat Karakter (Detailed)" << endl;
        cout << "3. Gacha Karakter" << endl;
        cout << "4. Lihat Karakter Saya" << endl;
        cout << "5. Logout" << endl;
        cout << "Pilihan: ";
        getline(cin, input);

        if (!ScanInput(input, pilihan)) {
            cout << "Input tidak valid! Masukkan angka." << endl;
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
                gachaKarakter();
                break;
            case 4:
                tampilkanKarakterUser();
                break;
            case 5:
                is_login = false;
                cout << "Logout berhasil." << endl;
                return;  // Keluar dari fungsi
            default:
                cout << "Pilihan tidak valid!" << endl;
        }
    }
}

// ===================== MAIN PROGRAM ===================== 
int main(int argc, char* argv[]) {
    muatKarakterDariCSV();
    muatMusuhDariCSV();
    muatUserDariCSV();
    // users[jumlahUser++] = {"demonia", "utakatik", true};  // User admin default

    int menu_utama;
    string input;

    while (true) {
        if (!is_login) {
            cout << "\n=== MENU UTAMA ===" << endl;
            cout << "1. Login" << endl;
            cout << "2. Register" << endl;
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
                case 2: registrasi(); break;
                case 3: 
                    cout << "Keluar dari program." << endl;
                    return 0;
                default:
                    cout << "Pilihan tidak valid! Program Eror pake Banget" << endl;
            }
        }
        else {
            if (is_admin) {
                adminMenu();
            }
            else {
                userMenu();
            }
        }
    }

    return 0;
}