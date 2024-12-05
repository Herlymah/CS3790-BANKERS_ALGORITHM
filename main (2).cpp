#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <sstream>

using namespace std;

// Global variables to store system state
int num_processes;
int num_resources;
vector<vector<int>> MAX;
vector<vector<int>> ALLOCATION;
vector<vector<int>> NEED;
vector<int> AVAILABLE;

// Calculate NEED matrix and initial ALLOCATION matrix (with zeros)
void initialize_matrices() {
    // Initialize ALLOCATION to zeros
    ALLOCATION.resize(num_processes, vector<int>(num_resources, 0));
    
    // Calculate NEED matrix
    NEED.resize(num_processes, vector<int>(num_resources));
    for (int i = 0; i < num_processes; ++i) {
        for (int j = 0; j < num_resources; ++j) {
            NEED[i][j] = MAX[i][j];  // Initially, NEED is the same as MAX
        }
    }
}

// Check if request can lead to a safe state
bool is_safe_state(const vector<int>& request, int process_id) {
    // Create temporary copies for simulation
    vector<vector<int>> temp_allocation = ALLOCATION;
    vector<vector<int>> temp_need = NEED;
    vector<int> temp_available = AVAILABLE;

    // First, allocate resources tentatively
    for (int j = 0; j < num_resources; ++j) {
        temp_available[j] -= request[j];
        temp_allocation[process_id][j] += request[j];
        temp_need[process_id][j] -= request[j];
    }

    // Safety algorithm
    vector<bool> finish(num_processes, false);
    vector<int> work = temp_available;

    for (int count = 0; count < num_processes; ++count) {
        bool found = false;
        for (int p = 0; p < num_processes; ++p) {
            if (!finish[p]) {
                bool can_allocate = true;
                for (int j = 0; j < num_resources; ++j) {
                    if (temp_need[p][j] > work[j]) {
                        can_allocate = false;
                        break;
                    }
                }

                if (can_allocate) {
                    // Pretend to finish this process
                    for (int j = 0; j < num_resources; ++j) {
                        work[j] += temp_allocation[p][j];
                    }
                    finish[p] = true;
                    found = true;
                    break;
                }
            }
        }

        if (!found) {
            // Could not find a process to allocate
            return false;
        }
    }

    return true;
}

// Print matrix with headers
void print_matrix(const vector<vector<int>>& matrix, const string& name) {
    cout << name << ":\n";
    for (const auto& row : matrix) {
        for (int val : row) {
            cout << setw(4) << val << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

// Print vector with header
void print_vector(const vector<int>& vec, const string& name) {
    cout << name << ": ";
    for (int val : vec) {
        cout << setw(4) << val << " ";
    }
    cout << "\n\n";
}

// Read input from file
void read_input(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Could not open file: " << filename << endl;
        exit(1);
    }

    // Read number of processes and resources
    file >> num_processes >> num_resources;

    // Resize MAX matrix
    MAX.resize(num_processes, vector<int>(num_resources));

    // Read MAX matrix
    for (int i = 0; i < num_processes; ++i) {
        for (int j = 0; j < num_resources; ++j) {
            file >> MAX[i][j];
        }
    }

    // Read AVAILABLE vector
    AVAILABLE.resize(num_resources);
    for (int j = 0; j < num_resources; ++j) {
        file >> AVAILABLE[j];
    }

    // Initialize ALLOCATION and NEED matrices
    initialize_matrices();
}

// Print Resource Allocation State
void print_resource_allocation_state() {
    cout << "===== RESOURCE ALLOCATION STATE =====\n";
    print_matrix(MAX, "MAX");
    print_matrix(ALLOCATION, "ALLOCATION");
    print_matrix(NEED, "NEED");
    print_vector(AVAILABLE, "AVAILABLE");
}

// Process resource requests
void process_requests() {
    while (true) {
        // Print resource allocation state
        print_resource_allocation_state();

        // Prompt for process selection
        int process_id;
        cout << "Enter process number (0-" << num_processes - 1 
              << ") or -1 to exit: ";
        cin >> process_id;

        // Exit condition
        if (process_id == -1) break;

        // Validate process number
        if (process_id < 0 || process_id >= num_processes) {
            cout << "Invalid process number. Try again.\n";
            continue;
        }

        // Prompt for request vector
        vector<int> request(num_resources);
        string request_line;
        cout << "Enter request vector for Process " << process_id << " (space-separated values): ";
        cin.ignore(); // Clear any previous newline
        getline(cin, request_line);

        // Parse request vector
        istringstream iss(request_line);
        for (int j = 0; j < num_resources; ++j) {
            if (!(iss >> request[j])) {
                cout << "Error: Not enough values in request vector!\n";
                goto request_failed;
            }

            // Validate request against NEED
            if (request[j] > NEED[process_id][j]) {
                cout << "Error: Request exceeds process's maximum need!\n";
                cout << "MAX[" << process_id << "][" << j << "]: " << MAX[process_id][j] << "\n";
                cout << "ALLOCATION[" << process_id << "][" << j << "]: " << ALLOCATION[process_id][j] << "\n";
                cout << "NEED[" << process_id << "][" << j << "]: " << NEED[process_id][j] << "\n";
                cout << "Requested: " << request[j] << "\n";
                goto request_failed;
            }

            //debugging
            // Validate request against AVAILABLE
            if (request[j] > AVAILABLE[j]) {
                cout << "Error: Not enough available resources!\n";
                cout << "AVAILABLE[" << j << "]: " << AVAILABLE[j] << "\n";
                cout << "Requested: " << request[j] << "\n";
                goto request_failed;
            }
        }

        // Check if request leads to safe state
        if (is_safe_state(request, process_id)) {
            // Update matrices
            for (int j = 0; j < num_resources; ++j) {
                AVAILABLE[j] -= request[j];
                ALLOCATION[process_id][j] += request[j];
                NEED[process_id][j] -= request[j];
            }
            cout << "Request ALLOWED. System remains in safe state.\n";
        } else {
            cout << "Request DENIED. Would lead to unsafe state.\n";
        }

        continue;

        // Label for request validation failures
        request_failed:
        cout << "Request processing halted due to validation error.\n";
    }
}

int main() {
    try {
        // Read input from file
        read_input("input.txt");

        // Process resource requests
        process_requests();

        // Final resource allocation state
        cout << "\nFinal ";
        print_resource_allocation_state();
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}