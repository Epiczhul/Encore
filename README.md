# Encore - Innovation on the Block
  
**Encore** is a high-performance, lightweight, plugin-capable Minecraft server software written in **C++**.
<div align="center">
  <a href="https://discord.gg/z3AwTHzxFy">
    <img src="https://img.shields.io/badge/Discord-5865F2?style=for-the-badge&logo=discord&logoColor=white"/>
  </a>
  <br>
</div>



> Pronunciation: **Encore** is pronounced like in french - [How to Pronounce encore](https://youtu.be/_TndRxbUXzI?si=IGWAAAUA23HAcXij)

---

## 🧠 To-Do

### 🎮 Core Features
- [x] Basic logging system
- [x] Server list ping response (MOTD, version, player count)
- [x] Player joining/login sequence (basic handshake)
- [x] Player movement handling
- [x] Keep-alive packet support
- [ ] Mojang authentication (online mode support)
- [x] Basic chat message handling
- [x] Entity metadata sync (for player models, health, etc.)

### 🌍 World System
- [x] Chunk data serialization/deserialization (Anvil or custom format)
- [ ] Basic world loading (read chunk files)
- [x] Basic world generation (flat, void)
- [x] Block change packet handling
- [ ] Block ticking / scheduled updates

### 🔄 Performance / Architecture
- [ ] Multithreading support (chunk loading, entity ticking, networking)
- [ ] Tick loop and scheduler system
- [ ] Thread-safe event system
- [ ] Plugin system API (C ABI or scripting)
- [ ] Efficient network packet pipeline

### 🌐 Networking
- [ ] Compression & encryption (as per protocol)
- [X] Login encryption handshake
- [ ] Full protocol version support (1.20+)
- [ ] Packet handler system with dispatcher

### 📦 Infrastructure
- [x] Config file system (YAML or JSON)
- [x] Command system (e.g., `/say`, `/stop`, `/gamemode`, plugin-extendable)
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
- Using `CMake (ninja)`
- Recommended compiler: `clang++` or `g++`
- Recommended ide: `CLion`

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


© Copyright 2025 - Epiczhul
