### **ResEdit: A Resource Editor for Sky: Children of the Light**

**Overview**

ResEdit is a mod for Sky: Children of the Light that allows users to modify game assets and resources during initialization time. It loads resource packs, similar to Minecraft, and features a priority system where resource packs at the bottom have lower priority than those at the top. Mod can be used with Canvas.

**Features**

* **Resource Pack Support:** Load and apply custom resource packs to modify game assets.
* **Priority System:** Control the order in which resource packs are applied to determine which modifications take precedence.

**Getting Started**

0. **Preparation**
   
   Please do one of the following bellow:
   * Add Android NDK path to your `ANDROID_NDK_HOME` environment variable
   * Add Android SDK path to your `ANDROID_HOME` environment variable and use `-DANDROID_NDK_VERSION={ndk_version}` flag when building the lib
  
   Step above is needed because project does not use gradle, because there is no need for additional building overhead since we only need to build native code
2. **Clone the Repository:**
   ```bash
   git clone https://github.com/gxosty/sky-resedit.git
   ```
3. **Build the Project:**
   ```bash
   cd sky-resedit
   mkdir build
   cd build
   cmake -GNinja ..   [-DANDROID_NDK_VERSION={ndk_version} if needed]
   cmake --build .. --parallel [parallel-compile-number]
   ```

**Contributing**

We welcome contributions from the community! If you're interested in contributing, please follow these guidelines:

* **Fork the Repository:** Create a fork of the ResEdit repository on GitHub.
* **Create a Branch:** Create a new branch for your feature or bug fix.
* **Make Changes:** Implement your changes and write clear commit messages.
* **Submit a Pull Request:** Submit a pull request to the main repository, detailing your changes and addressing any feedback.
