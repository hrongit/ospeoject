#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

const int DISK_SIZE = 100;
const int BLOCK_SIZE = 10;

// Struct representing a file
struct File {
    string name;
    int start_block;
    int size;
};

// Struct representing a block on the disk
struct DiskBlock {
    int start_index;
    bool used;
};

// Function to initialize the disk with free blocks
vector<DiskBlock> initialize_disk() {
    vector<DiskBlock> disk;
    for (int i = 0; i < DISK_SIZE / BLOCK_SIZE; i++) {
        disk.push_back({ i * BLOCK_SIZE, false });
    }
    return disk;
}

// Function to allocate space for a file using contiguous allocation algorithm
bool allocate_contiguous(vector<DiskBlock>& disk, File& file) {
    int num_blocks_needed = file.size / BLOCK_SIZE + ((file.size % BLOCK_SIZE) ? 1 : 0);
    int start_index = -1;
    for (int i = 0; i < disk.size() - num_blocks_needed + 1; i++) {
        bool found = true;
        for (int j = 0; j < num_blocks_needed; j++) {
            if (disk[i + j].used) {
                found = false;
                break;
            }
        }
        if (found) {
            start_index = i;
            break;
        }
    }
    if (start_index == -1) {
        return false;
    }
    file.start_block = start_index;
    for (int i = start_index; i < start_index + num_blocks_needed; i++) {
        disk[i].used = true;
    }
    return true;
}

// Function to deallocate space for a file
void deallocate(vector<DiskBlock>& disk, File& file) {
    int start_index = file.start_block;
    int num_blocks_needed = file.size / BLOCK_SIZE + ((file.size % BLOCK_SIZE) ? 1 : 0);
    for (int i = start_index; i < start_index + num_blocks_needed; i++) {
        disk[i].used = false;
    }
}

// Function to print the current state of the disk
void print_disk(vector<DiskBlock>& disk) {
    for (int i = 0; i < disk.size(); i++) {
        if (i % (DISK_SIZE / BLOCK_SIZE) == 0) {
            cout << endl;
        }
        cout << (disk[i].used ? "X" : "-") << " ";
    }
    cout << endl;
}

// Function to calculate the fragmentation of the disk
double calculate_fragmentation(vector<DiskBlock>& disk) {
    double free_blocks = 0;
    int num_fragments = 0;
    bool in_fragment = false;
    for (int i = 0; i < disk.size(); i++) {
        if (!disk[i].used) {
            free_blocks++;
            if (!in_fragment) {
                in_fragment = true;
                num_fragments++;
            }
        }
        else {
            in_fragment = false;
        }
    }
    return num_fragments == 0 ? 0 : free_blocks / num_fragments;
}

int main() {
    vector<DiskBlock> disk = initialize_disk();
    vector<File> files;
    int time_units = 10;

    for (int t = 1; t <= time_units; t++) {
        cout << "Time unit " << t << endl;

        // Create a new file
        File new_file;
            cout << "Enter name of new file: ";
    cin >> new_file.name;
    cout << "Enter size of new file (in bytes): ";
    cin >> new_file.size;

    // Allocate space for the new file
    if (allocate_contiguous(disk, new_file)) {
        files.push_back(new_file);
        cout << "File " << new_file.name << " created." << endl;
    }
    else {
        cout << "Error: Not enough space on disk to create file " << new_file.name << "." << endl;
    }

    // Deallocate space for a random file
    if (!files.empty()) {
        int index = rand() % files.size();
        File file_to_delete = files[index];
        deallocate(disk, file_to_delete);
        files.erase(files.begin() + index);
        cout << "File " << file_to_delete.name << " deleted." << endl;
    }

    // Rename a random file
    if (!files.empty()) {
        int index = rand() % files.size();
        File& file_to_rename = files[index];
        string new_name;
        cout << "Enter new name for file " << file_to_rename.name << ": ";
        cin >> new_name;
        file_to_rename.name = new_name;
        cout << "File renamed to " << new_name << "." << endl;
    }

    // Move a random file
    if (!files.empty()) {
        int index = rand() % files.size();
        File& file_to_move = files[index];
        int new_start_block;
        cout << "Enter new start block for file " << file_to_move.name << " (current start block is " << file_to_move.start_block << "): ";
        cin >> new_start_block;
        int num_blocks_needed = file_to_move.size / BLOCK_SIZE + ((file_to_move.size % BLOCK_SIZE) ? 1 : 0);
        if (new_start_block < 0 || new_start_block + num_blocks_needed > disk.size()) {
            cout << "Error: Invalid start block." << endl;
        }
        else {
            deallocate(disk, file_to_move);
            file_to_move.start_block = new_start_block;
            allocate_contiguous(disk, file_to_move);
            cout << "File moved to start block " << new_start_block << "." << endl;
        }
    }

    // Print the current state of the disk
    print_disk(disk);

    // Print the fragmentation of the disk
    double fragmentation = calculate_fragmentation(disk);
    cout << "Fragmentation: " << fixed << setprecision(2) << fragmentation << endl;
}

return 0; }

       
