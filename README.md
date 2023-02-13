# WiFi configuration for esp-idf, as simple as possible

This repository contains an esp-idf component with a single function which
configures WiFi as a station (client), with credentials provided through
configuration options at compile time.

## Usage

Clone this repository into `components`, or add it as a git submodule.

in main.cpp, add 

```cpp
#include "simple_wifi.hpp"
```

and call `simple_wifi::start()` in `app_main`:

```cpp
extern "C" void app_main() {
    simple_wifi::start();

    // your application code here
}
```

This configures the NVS system (potentially **deleting the contents** if the
contents are not valid!) and configures the WiFi.

## Configuration

To configure, either:

-   use menuconfig: run `idf.py menuconfig` and navigate to `Component config ->
    simple wifi`. Edit the settings, save and quit.

-   edit `sdkconfig.defaults` before building the project for the first time. Add lines like:

    ```
    CONFIG_SIMPLE_WIFI_SSID="my-ssid"
    CONFIG_SIMPLE_WIFI_PASSWORD="my-password"
    ```

    I typically set `CONFIG_LWIP_LOCAL_HOSTNAME` to the desired host name too.

## Limitations

This does exactly what I want and nothing more. As such there are limitations
which are not likely to be resolved:

- It could well be insecure -- read and understand the code yourself.

- Only supports a single access point and key pair.

- Only configurable at compile time.

These limitations could be resolved; open an issue / PR:

- It configures NVS and possibly deletes the contents. Options could be added
  to skip this step to allow customisation, or to configure WiFi in a way which
  doesn't use the NVS (which is generally a lot slower to set up).

- Only supports WPA2.

- Only supports DHCP, not static IPs.

- Limited esp-idf version support: I generally keep this up to date with the
  latest version, but every version seems to break something. Check the commit
  history to see changes I've made for each version if you want to use it with
  something older.

- API is only usable from C++, not C.

- Reconnection is not very sophisticated -- it's generally reliable, though.

## License

Licensed under GNU Affero General Public License v3.0. See `LICENSE`.
