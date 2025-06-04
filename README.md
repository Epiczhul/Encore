# Encore - Innovation on the Block

**Encore** is a high-performance, lightweight, plugin-capable Minecraft server software written in **C++**.
<div align="center">
  <img src="https://img.shields.io/badge/Discord-5865F2?style=for-the-badge&logo=discord&logoColor=white"/>
  <br>
</div>


> Pronunciation: **Encore** is pronounced like in french - [How to Pronounce encore](https://youtu.be/_TndRxbUXzI?si=IGWAAAUA23HAcXij)

---

## 🧠 To-Do

### 🎮 Core Features
- [x] Basic logging system
- [x] Server list ping response (MOTD, version, player count)
- [x] Player joining/login sequence (basic handshake)
- [ ] Player movement handling
- [ ] Keep-alive packet support
- [ ] Mojang authentication (online mode support)
- [ ] Basic chat message handling
- [ ] Entity metadata sync (for player models, health, etc.)

### 🌍 World System
- [ ] Chunk data serialization/deserialization (Anvil or custom format)
- [ ] Basic world loading (read chunk files)
- [ ] Basic world generation (flat, void)
- [ ] Block change packet handling
- [ ] Block ticking / scheduled updates

### 🔄 Performance / Architecture
- [ ] Multithreading support (chunk loading, entity ticking, networking)
- [ ] Tick loop and scheduler system
- [ ] Thread-safe event system
- [ ] Plugin system API (C ABI or scripting)
- [ ] Efficient network packet pipeline

### 🌐 Networking
- [ ] Compression & encryption (as per protocol)
- [ ] Login encryption handshake
- [ ] Full protocol version support (1.20+)
- [ ] Packet handler system with dispatcher

### 📦 Infrastructure
- [ ] Config file system (YAML or JSON)
- [ ] Command system (e.g., `/say`, `/stop`, plugin-extendable)
- [ ] Permissions system (optional integration)
- [ ] Metrics/log output (tick time, memory usage)

---

## 🧪 Optional Future Features
- [ ] Web UI dashboard (Rust/React frontend?)
- [ ] Rust or Lua plugin scripting
- [ ] Custom lightweight chunk format (compressed, LOD?)
- [ ] Docker-ready deployment tools

---

## 🛠 Build System
- Using `CMake`
- Recommended compiler: `clang++` or `g++`

## 🔗 License
This project will be open source under the **[GPLv3 License](https://www.gnu.org/licenses/gpl-3.0.en.html)**.

---

## 🤝 Contributing
Pull requests and contributions are welcome!

---

## 📬 Contact
Want to collaborate or follow progress? Open an issue, pull request or join our [Discord](https://discord.gg/z3AwTHzxFy)!

---

**Encore** – Innovation on the Block ✨
