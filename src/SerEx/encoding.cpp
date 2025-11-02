export module serex.encoding;
import std;


namespace serex {
    inline auto hex_value(const char c) -> std::uint8_t {
        if (c >= '0' && c <= '9') return c - '0';
        if (c >= 'a' && c <= 'f') return c - 'a' + 10;
        if (c >= 'A' && c <= 'F') return c - 'A' + 10;
        throw std::invalid_argument("Invalid hex digit");
    }

    export inline auto to_hex(
        const std::span<std::uint8_t> data) ->
        std::string {
        // Prepare the hexadecimal characters.
        static constexpr char hex_chars[] = "0123456789abcdef";
        auto hex_data = std::string();
        hex_data.resize(data.size() * 2);

        // Convert each byte to its hexadecimal representation.
        for (std::size_t i = 0; i < data.size(); ++i) {
            hex_data[i * 2] = hex_chars[data[i] >> 4 & 0x0F];
            hex_data[i * 2 + 1] = hex_chars[data[i] & 0x0F];
        }

        // Return the hexadecimal string.
        return hex_data;
    }

    export inline auto from_hex(
        std::string const &hex_str) ->
        std::string {
        // Prepare the output byte array.
        auto data = std::string();
        data.resize(hex_str.size() / 2);

        // Convert the hexadecimal string back to bytes.
        for (auto i = 0uz; i < data.size(); ++i) {
            const auto hi = hex_value(hex_str[2 * i]);
            const auto lo = hex_value(hex_str[2 * i + 1]);
            data[i] = (hi << 4) | lo;
        }

        // Return the byte array.
        return data;
    }
}
