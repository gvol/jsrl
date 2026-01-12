# Quick Start

```cpp
#include "jsrl.hpp"
#include <iostream>
#include <sstream>

using namespace jsrl;
using namespace jsrl::literals;

int main() {
    // Parse JSON from string
    std::istringstream iss(R"({"name": "John", "age": 30, "active": true})");
    Json json;
    iss >> json;

    // Or use string literals
    auto json2 = R"({"items": [1, 2, 3]})"_Json;

    // Access data
    std::cout << json["name"].as_string() << std::endl;  // John
    std::cout << json["age"].as_number_sint() << std::endl;  // 30
    std::cout << json["active"].as_bool() << std::endl;  // true

    // Safe access with defaults
    std::string country = json.get_string("country", "US");

    // Iterate arrays
    for (auto const& item : json2["items"].as_array()) {
        std::cout << item.as_number_sint() << " ";
    }
    // Output: 1 2 3

    // Iterate objects
    for (auto const& [key, value] : json.as_object()) {
        std::cout << key << ": " << value << std::endl;
    }

    // Output JSON
    std::cout << json << std::endl;  // Compact: {"name":"John",...}
    std::cout << pretty_print(json).indent("  ") << std::endl;  // Formatted

    return 0;
}
```

## Creating JSON

### Constructing Objects

```cpp
// Using ObjectBody initializer
auto person = Json(Json::ObjectBody{
    { "id", Json(12345) },
    { "name", Json("Jane Doe") },
    { "email", Json("jane@example.com") },
    { "active", Json(true) }
});

// Dynamic construction
Json::ObjectBody obj;
jsrl::insert(obj, "timestamp", Json(1638360000));
jsrl::insert(obj, "message", Json("Hello, World!"));
Json event(obj);
```

### Constructing Arrays

```cpp
// Using ArrayBody initializer
auto numbers = Json(Json::ArrayBody{
    Json(1), Json(2), Json(3), Json(5), Json(8)
});

// Dynamic construction
Json::ArrayBody arr;
for (int i = 0; i < 10; ++i) {
    jsrl::push_back(arr, Json(i * i));
}
Json squares(arr);
```

### Nested Structures

```cpp
auto api_response = Json(Json::ObjectBody{
    { "status", Json("success") },
    { "data", Json(Json::ObjectBody{
        { "user", Json(Json::ObjectBody{
            { "id", Json(123) },
            { "username", Json("jdoe") }
        })},
        { "permissions", Json(Json::ArrayBody{
            Json("read"), Json("write")
        })}
    })},
    { "timestamp", Json(1638360000) }
});
```

## Type Checking and Conversion

```cpp
Json value = /* ... */;

// Type queries
if (value.is_null())   { /* ... */ }
if (value.is_bool())   { /* ... */ }
if (value.is_number()) { /* ... */ }
if (value.is_string()) { /* ... */ }
if (value.is_array())  { /* ... */ }
if (value.is_object()) { /* ... */ }

// Extracting values (throws TypeError if wrong type)
bool b = value.as_bool();
std::string s = value.as_string();
long long i = value.as_number_sint();
unsigned long long u = value.as_number_uint();
double d = value.as_number_float();
Json::ArrayBody const& arr = value.as_array();
Json::ObjectBody const& obj = value.as_object();

// Safe extraction with defaults (no exceptions)
std::string name = json.get_string("name", "Anonymous");
int count = json.get_number_sint("count", 0);
bool enabled = json.get_bool("enabled", false);
```

## Modifying JSON (Immutable Style)

Since JSRL entities are immutable, modifications create new structures that share unchanged parts:

```cpp
#include "jsrl_mod.hpp"

auto json = R"({"counter": 0, "items": [1, 2, 3]})"_Json;

// Modify using the mod() proxy
mod(json)["counter"] = 42;                    // Set object key
mod(json)["items"][0] = 100;                  // Set array element
mod(json)["new_key"] = "new_value";           // Add new key
mod(json)["items"].push_back(4);              // Append to array
mod(json)["old_key"].erase();                 // Remove key
mod(json).erase_keys({"key1", "key2"});       // Remove multiple keys

// The original json is now modified (handle points to new structure)
std::cout << json << std::endl;
```

## Pretty Printing

```cpp
#include "jsrlpp.hpp"

Json data = /* ... */;

// Basic pretty print
std::cout << pretty_print(data) << std::endl;

// Customized formatting
std::cout << pretty_print(data)
    .indent("    ")                           // 4-space indent
    .base_indent(1)                           // Start at indent level 1
    .first_keys({"id", "name"})               // Show these keys first
    .comma_newline("\n")                      // Comma placement
    .colon_space(": ")                        // Colon spacing
    .sort_keys_alphabetic()                   // Sort keys alphabetically
    .set_ascii_utf8(false)                    // Output UTF-8 directly
    << std::endl;

// One-line output
std::cout << pretty_print(data).one_line() << std::endl;

// Control numeric precision
std::cout << pretty_print(data)
    .set_float_precision(FloatPrecision::LooseFloat)  // Reduce precision
    << std::endl;
```

## High-Fidelity Numbers

JSRL provides `GeneralNumber` for exact decimal representation:

```cpp
// Parse a number with many decimal places
auto json = R"({"pi": 3.14159265358979323846264338327950288})"_Json;

// Extract as GeneralNumber (preserves exact representation)
GeneralNumber precise = json["pi"].as_general_number();

// Convert to built-in types as needed
long double ld = precise.as_long_double();
std::string str = precise.to_string();

// Round-trip preservation
Json original = Json::parse("1.23456789012345678901234567890");
std::string encoded = encode(original);
// encoded == "1.23456789012345678901234567890" (exact match)
```

## Error Handling

JSRL uses exceptions for error conditions:

```cpp
try {
    Json json = Json::parse(input);

    std::string name = json["user"]["name"].as_string();
    int age = json["user"]["age"].as_number_sint();

} catch (Json::ParseError const& e) {
    // Invalid JSON syntax
    std::cerr << "Parse error: " << e.what() << std::endl;

} catch (Json::KeyError const& e) {
    // Missing key or index
    std::cerr << "Key not found: " << e.what() << std::endl;

} catch (Json::TypeError const& e) {
    // Type mismatch (e.g., calling as_string() on a number)
    std::cerr << "Type error: " << e.what() << std::endl;

} catch (Json::Error const& e) {
    // Base exception for all JSRL errors
    std::cerr << "JSON error: " << e.what() << std::endl;
}
```

## What Makes JSRL Different

### Immutability Enables Safe Concurrency

Unlike most JSON libraries, JSRL makes immutability a core feature:

```cpp
Json config = load_config();

// Can safely share across threads - no locks needed
std::thread t1([config]() { process_data(config); });
std::thread t2([config]() { validate_config(config); });
std::thread t3([config]() { log_config(config); });

// Copying is cheap (just copies a shared_ptr)
Json copy = config;  // No deep copy, shares underlying data
```

### Number Fidelity Priority

Many JSON libraries silently lose precision. JSRL doesn't:

```cpp
// Other libraries might lose precision:
// Input:  {"value": 1.23456789012345678901234567890}
// Output: {"value": 1.2345678901234568}  ❌

// JSRL preserves exact representation:
auto json = R"({"value": 1.23456789012345678901234567890})"_Json;
std::cout << json << std::endl;
// Output: {"value": 1.23456789012345678901234567890}  ✓

// Integer types are preserved (useful for cross-language serialization)
Json i = Json(42);
i.is_number_integer();  // true
i.is_number_signed();   // true

Json u = Json(42u);
u.is_number_unsigned();  // true
```

### Insertion Order Preservation

Objects use `vector<pair<string,Json>>` instead of `map`:

```cpp
auto json = R"({"z": 1, "a": 2, "m": 3})"_Json;

for (auto const& [key, value] : json.as_object()) {
    std::cout << key << " ";
}
// Output: z a m (insertion order preserved)

// Can still do efficient lookup
if (json.has_key("a")) {
    Json value = json["a"];
}

// Convert to map if needed
std::map<std::string, Json> map = json.as_map();
```

### Stream-Based I/O

Integrates naturally with C++ iostreams:

```cpp
// Parse from any input stream
std::ifstream file("config.json");
Json config;
file >> config;

// Parse from string stream
std::istringstream iss(jsonString);
Json data;
iss >> data;

// Write to any output stream
std::ofstream outfile("output.json");
outfile << pretty_print(data);

// Write to string stream
std::ostringstream oss;
oss << data;
std::string result = oss.str();
```

## Common Patterns

### Configuration Files

```cpp
Json load_config(std::string const& path) {
    std::ifstream file(path);
    Json config;
    file >> config;
    return config;
}

void use_config(Json const& config) {
    std::string host = config.get_string("host", "localhost");
    int port = config.get_number_sint("port", 8080);
    bool ssl = config.get_bool("ssl", false);

    // Safe nested access
    if (config.has_key("database") && config["database"].is_object()) {
        Json db = config["database"];
        std::string url = db.get_string("url", "");
        int pool_size = db.get_number_sint("pool_size", 10);
    }
}
```

### API Request/Response

```cpp
Json create_api_request(int user_id, std::string const& action) {
    return Json(Json::ObjectBody{
        { "api_version", Json("2.0") },
        { "timestamp", Json(std::time(nullptr)) },
        { "request", Json(Json::ObjectBody{
            { "user_id", Json(user_id) },
            { "action", Json(action) },
            { "parameters", Json(Json::ObjectBody{}) }
        })}
    });
}

void handle_api_response(Json const& response) {
    if (!response.is_object()) {
        throw std::runtime_error("Invalid response");
    }

    std::string status = response.get_string("status", "error");
    if (status == "success" && response.has_key("data")) {
        process_data(response["data"]);
    } else if (status == "error" && response.has_key("message")) {
        std::cerr << "API error: "
                  << response["message"].as_string()
                  << std::endl;
    }
}
```

### Data Transformation

```cpp
Json transform_records(Json const& input) {
    if (!input.is_array()) {
        throw std::runtime_error("Expected array");
    }

    Json::ArrayBody results;
    for (auto const& record : input.as_array()) {
        if (!record.is_object()) continue;

        // Transform each record
        Json::ObjectBody transformed{
            { "id", record["id"] },
            { "name", record.get("full_name", record.get("name", Json())) },
            { "timestamp", Json(std::time(nullptr)) }
        };

        jsrl::push_back(results, Json(transformed));
    }

    return Json(results);
}
```

