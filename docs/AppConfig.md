# AppConfig - Global Configuration Management

## Overview

The `AppConfig` class provides a thread-safe singleton for managing global application configuration parameters loaded from a JSON file. It is designed to be initialized once at application startup and accessed throughout the application lifetime.

## Features

- **Thread-safe**: All operations are protected by mutexes for concurrent access
- **Singleton pattern**: Single global instance accessed via `std::shared_ptr`
- **JSON-based**: Uses Boost.JSON for reading/writing configuration files
- **Extensible**: Easy to add new configuration parameters
- **Exception-safe**: Comprehensive error handling with descriptive messages

## Location

- **Header**: `include/rm/AppConfig.h`
- **Implementation**: `src/rm/AppConfig.cpp`
- **Namespace**: `rm`

## Configuration Format

The configuration file is a JSON object with the following structure:

```json
{
    "log-level": "info",
    "data-location": "./data"
}
```

### Current Configuration Parameters

| Parameter | Type | Default | Description |
|-----------|------|---------|-------------|
| `log-level` | string | `"info"` | Logging level (trace, debug, info, warn, error, critical, off) |
| `data-location` | string | `"./data"` | Path to the application data directory |

## Usage

### 1. Initialization

The configuration must be initialized once at application startup, before any access:

```cpp
#include <AppConfig.h>

int main() {
    try {
        // Initialize the configuration from a JSON file
        rm::AppConfig::initialize("config.json");

        // Rest of your application...
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to initialize config: " << e.what() << std::endl;
        return 1;
    }
}
```

### 2. Accessing Configuration

Access the configuration instance anywhere in your application:

```cpp
#include <AppConfig.h>

void someFunction() {
    // Get the configuration instance
    auto config = rm::AppConfig::instance();

    // Read configuration values
    std::string log_level = config->logLevel();
    std::string data_path = config->dataLocation();

    // Use the values...
}
```

### 3. Modifying Configuration

Configuration values can be modified at runtime in a thread-safe manner:

```cpp
auto config = rm::AppConfig::instance();

// Modify values (thread-safe)
config->setLogLevel("debug");
config->setDataLocation("/new/data/path");

// Optionally save the modified configuration
config->save("config_updated.json");
```

### 4. Saving Configuration

Save the current configuration to a JSON file:

```cpp
auto config = rm::AppConfig::instance();

// Save to the original file
config->save();

// Or save to a different file
config->save("config_backup.json");
```

### 5. Getting JSON Representation

Get the configuration as a Boost.JSON object:

```cpp
auto config = rm::AppConfig::instance();

boost::json::object json_config = config->toJson();
std::cout << boost::json::serialize(json_config) << std::endl;
```

## Example Application

A complete example application is provided in `apps/config_example.cpp`:

```bash
# Build the example
cmake --build . --target config_example

# Run the example
./config_example config.json
```

## Thread Safety

The `AppConfig` class provides thread safety through:

1. **Singleton initialization**: Protected by `instance_mutex_`
2. **Configuration access**: All getters use const member functions
3. **Configuration modification**: All setters use `std::lock_guard` with `mutex_`
4. **File operations**: Protected by mutex during JSON serialization

## Error Handling

The class throws `std::runtime_error` in the following cases:

- **Initialization errors**:
  - `initialize()` called more than once
  - Configuration file cannot be opened
  - Configuration file is empty
  - JSON parsing fails
  - JSON root is not an object

- **Access errors**:
  - `instance()` called before `initialize()`

- **Save errors**:
  - No file path specified
  - File cannot be opened for writing

## Adding New Configuration Parameters

To add a new configuration parameter:

### 1. Add the member variable in `AppConfig.h`:

```cpp
private:
    std::string log_level_{"info"};
    std::string data_location_{"./data"};
    std::string new_parameter_{"default_value"};  // Add this
```

### 2. Add getter and setter methods in `AppConfig.h`:

```cpp
public:
    const std::string& newParameter() const {
        return new_parameter_;
    }

    void setNewParameter(const std::string& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        new_parameter_ = value;
    }
```

### 3. Update `fromJson()` in `AppConfig.cpp`:

```cpp
void AppConfig::fromJson(const json::object& json_obj) {
    std::lock_guard<std::mutex> lock(mutex_);

    // Existing parameters...

    // Add new parameter
    if (json_obj.contains("new-parameter")) {
        const json::value& val = json_obj.at("new-parameter");
        if (val.is_string()) {
            new_parameter_ = val.as_string().c_str();
        }
    }
}
```

### 4. Update `toJson()` in `AppConfig.cpp`:

```cpp
json::object AppConfig::toJson() const {
    std::lock_guard<std::mutex> lock(mutex_);

    json::object obj;
    obj["log-level"] = log_level_;
    obj["data-location"] = data_location_;
    obj["new-parameter"] = new_parameter_;  // Add this

    return obj;
}
```

### 5. Update your `config.json`:

```json
{
    "log-level": "info",
    "data-location": "./data",
    "new-parameter": "value"
}
```

## Integration with Logging

The `log-level` parameter is designed to integrate with spdlog:

```cpp
#include <AppConfig.h>
#include <spdlog/spdlog.h>

void setupLogging() {
    auto config = rm::AppConfig::instance();
    std::string level = config->logLevel();

    // Map string to spdlog level
    if (level == "trace") spdlog::set_level(spdlog::level::trace);
    else if (level == "debug") spdlog::set_level(spdlog::level::debug);
    else if (level == "info") spdlog::set_level(spdlog::level::info);
    else if (level == "warn") spdlog::set_level(spdlog::level::warn);
    else if (level == "error") spdlog::set_level(spdlog::level::err);
    else if (level == "critical") spdlog::set_level(spdlog::level::critical);
    else if (level == "off") spdlog::set_level(spdlog::level::off);
}
```

## Testing

The example application `config_example` can be used for testing:

```bash
# Test with default config.json
./config_example

# Test with custom config file
./config_example path/to/custom_config.json

# The example will:
# 1. Load the configuration
# 2. Display current values
# 3. Modify a value
# 4. Save to a new file
```

## Dependencies

- **Boost.JSON**: For JSON parsing and serialization
- **Boost.System**: For error handling
- **C++23**: For modern C++ features (using the project's standard)
- **Standard Library**: `<memory>`, `<mutex>`, `<string>`, `<fstream>`, `<sstream>`

## Design Rationale

### Why Singleton?

The configuration is truly global and should have exactly one instance throughout the application lifetime. The singleton pattern ensures:
- Single source of truth
- Controlled initialization
- Prevention of duplicate configuration objects

### Why std::shared_ptr?

Using `std::shared_ptr` provides:
- Automatic lifetime management
- Safe sharing across threads
- Prevention of dangling references
- Ability to extend lifetime if needed

### Why Thread-Safe?

Configuration may be accessed from multiple threads:
- Main application thread
- REST server threads
- Background worker threads
- Logger threads

The mutex-based synchronization ensures all accesses are safe.

## Future Enhancements

Potential improvements for future versions:

1. **Configuration validation**: Schema-based validation of configuration files
2. **Hot reloading**: Watch for file changes and reload automatically
3. **Environment variables**: Override configuration with environment variables
4. **Nested configuration**: Support hierarchical configuration structures
5. **Type-safe accessors**: Template-based getters with automatic type conversion
6. **Configuration profiles**: Support for dev/staging/production profiles
7. **Encrypted configuration**: Support for encrypted sensitive configuration values

## License

Part of the RMCE project.
