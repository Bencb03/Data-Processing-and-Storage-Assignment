#include <iostream>
#include <unordered_map>
#include <string>
#include <stdexcept>

class InMemoryDB {
private:
    std::unordered_map<std::string, int> main_store;
    std::unordered_map<std::string, int> transaction_store;
    bool transaction_active;

public:
    InMemoryDB() : transaction_active(false) {}

    void begin_transaction() {
        if (transaction_active) {
            throw std::runtime_error("Transaction already in progress");
        }
        transaction_active = true;
        transaction_store.clear();
    }

    void put(const std::string& key, int value) {
        if (!transaction_active) {
            throw std::runtime_error("No transaction in progress");
        }
        transaction_store[key] = value;
    }

    int get(const std::string& key) {
        if (transaction_store.find(key) != transaction_store.end()) {
            return transaction_store[key];
        }
        if (main_store.find(key) != main_store.end()) {
            return main_store[key];
        }
        throw std::runtime_error("Key not found");
    }

    void commit() {
        if (!transaction_active) {
            throw std::runtime_error("No transaction to commit");
        }
        for (const auto& kv : transaction_store) {
            main_store[kv.first] = kv.second;
        }
        transaction_active = false;
        transaction_store.clear();
    }

    void rollback() {
        if (!transaction_active) {
            throw std::runtime_error("No transaction to rollback");
        }
        transaction_active = false;
        transaction_store.clear();
    }
};

int main() {
    InMemoryDB db;

    // Testing the database operations
    try {
        std::cout << "Get A: " << db.get("A") << std::endl;  // Should throw 'Key not found'
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    try {
        db.put("A", 5);  // Should throw 'No transaction in progress'
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    db.begin_transaction();
    db.put("A", 5);  // Value of A set to 5 in transaction

    try {
        std::cout << "Get A in Transaction: " << db.get("A") << std::endl;  // Should return 5
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    db.put("A", 6);  // Update A's value to 6 within the transaction
    db.commit();

    try {
        std::cout << "Get A after commit: " << db.get("A") << std::endl;  // Should return 6
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    try {
        db.commit();  // Should throw 'No transaction to commit'
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    try {
        db.rollback();  // Should throw 'No transaction to rollback'
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    try {
        std::cout << "Get B: " << db.get("B") << std::endl;  // Should throw 'Key not found'
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    db.begin_transaction();
    db.put("B", 10);  // Set B's value to 10 within transaction
    db.rollback();

    try {
        std::cout << "Get B after rollback: " << db.get("B") << std::endl;  // Should throw 'Key not found'
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
