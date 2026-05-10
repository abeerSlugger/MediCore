#ifndef STORAGE_H
#define STORAGE_H

template <typename T>
class Storage {
    T   data[100];
    int count;
public:
    Storage() : count(0) {}
    bool add(const T& item) { if (count >= 100) return false; data[count++] = item; return true; }
    bool removeAt(int idx) { if (idx < 0 || idx >= count) return false; for (int i = idx; i < count - 1; i++) data[i] = data[i + 1]; count--; return true; }
    bool removeById(int id) { for (int i = 0; i < count; i++) if (data[i].getId() == id) return removeAt(i); return false; }
    T* findById(int id) { for (int i = 0; i < count; i++) if (data[i].getId() == id) return &data[i]; return nullptr; }
    T& get(int i) { return data[i]; }
    T* getAll() { return data; }
    int  size()  const { return count; }
    void clear() { count = 0; }
    int  maxId() const { int mx = 0; for (int i = 0; i < count; i++) if (data[i].getId() > mx) mx = data[i].getId(); return mx; }
    bool exists(int id) { return findById(id) != nullptr; }
};

#endif