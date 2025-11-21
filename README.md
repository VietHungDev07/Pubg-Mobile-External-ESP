# Pubg-Mobile-External-ESP


An educational External ESP project for PUBG Mobile (Gameloop).
This project demonstrates how to build a clean external cheat using memory reading, pattern scanning, and overlay rendering (ImGui) — without using SDK dumps.
> ⚠️ **This project is strictly for educational and research purposes.  
> We do NOT encourage cheating in online games.**

# 📌Features
> - **External** — No injection, no DLL, no modification of game files  
> - **Auto-find GWorld & GName using Pattern Scan**  
> - **Entity ESP**  
>   - 2D Box ESP  
>   - Skeleton ESP  
>   - Health Bar  
>   - Player Name  
>   - Distance  
> - **Team Check**  
> - **ImGui Overlay Menu**  
> - **Fallback patterns pre-integrated for next updates**  
> - **Pattern-based engine scanning** (works even if offsets change)
> - <img width="1920" height="1080" alt="Screenshot (1)" src="https://github.com/user-attachments/assets/58304765-519f-42c4-b37c-c1c7b5cb0445" />

# 🛠 Requirements
> - Windows 10 / 11  
> - Visual Studio 2022  
> - C++17 support  
> - Gameloop 4.x (aow_exe.exe)  
> - Administrator privileges

# 🚀 How to Build
> 1. Install **Visual Studio 2022**  
> 2. Open the solution `ImGuiExternal.sln`  
> 3. Select:  
>    ```
>    Release | x64
>    ```  
> 4. Press **Build → Build Solution**  
> 5. Run the compiled `.exe` **as Administrator**
>
> ✔ It's a normal C++ project — no special configuration needed.

# 🔧 How the Project Updates Itself (NO SDK NEEDED)
> PUBG Mobile updates frequently.  
> Normally you need SDK → GWorld → GName → Offsets → Rebuild.
>
> This project avoids that by using **signature patterns**.
>
> ### Example GWorld patterns included:
>
> ```
> GWorld: 8B 52 ? C7 45 ? ? ? ? ? 8B 0D
> GWorld: 8B 52 ? 89 D3 89 74 24
> ```
>
> The pattern scanner will:
> 1. Search the game memory  
> 2. Find the address using wildcard matching (`?`)  
> 3. Automatically update:
>    - `GWorld`
>    - `GNames`
>    - Actor offsets  
>    - Component offsets  
>
> ✔ When PUBG updates, you only need to change **patterns** → NOT rebuild SDK.





