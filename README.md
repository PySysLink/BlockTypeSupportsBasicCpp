# BlockTypeSupportsBasicCpp

**BlockTypeSupportsBasicCpp** is a repository that provides support for C++ blocks within the PySysLink simulation framework. It builds on top of the core PySysLinkBase library and uses the block definitions from the BlockTypesBasicCpp repository. Additionally, it implements a plug‑in loader that dynamically loads additional block libraries (e.g. BlockLibrariesBasicBlocksBasicCpp) so that new block implementations can be added without recompiling the core system.

## Repository Relationships

- **PySysLinkBase**  
  The core simulation engine. It defines the main interfaces (such as ISimulationBlock, ModelParser, etc.) and handles the simulation workflow.

- **BlockTypesBasicCpp**  
  Defines the fundamental interfaces and base classes for blocks implemented in C++. This repository remains independent of PySysLinkBase to keep the block definitions decoupled from the simulation engine.

- **BlockTypeSupportsBasicCpp**  
  Implements support for C++ block types. It depends on both PySysLinkBase and BlockTypesBasicCpp. Its key role is to serve as a plug‑in loader for block libraries and to convert block definitions (from BlockTypesBasicCpp) into fully functional simulation blocks.

- **BlockLibrariesBasicBlocksBasicCpp**  
  Contains concrete implementations of blocks (e.g. Constant, Sumator, Integrator) conforming to the BlockTypesBasicCpp interfaces. These are loaded into BlockTypeSupportsBasicCpp as plug‑ins.

## Installation Order

To use BlockTypeSupportsBasicCpp (and thus gain access to the full range of available block libraries), you must install the components in the following order:

1. **Install PySysLinkBase:**  
   Clone and install the core simulation engine as described on its repository [PySysLinkBase](https://github.com/PySysLink/PySysLinkBase).

2. **Install BlockTypesBasicCpp:**  
   Clone and install the C++ block interface definitions:
   ```bash
   git clone https://github.com/PySysLink/BlockTypesBasicCpp.git
   cd BlockTypesBasicCpp
   mkdir build && cd build
   cmake ..
   make
   sudo make install
   ```

3. **Install BlockTypeSupportsBasicCpp:**  
   Clone and install this repository:
   ```bash
   git clone https://github.com/PySysLink/BlockTypeSupportsBasicCpp.git
   cd BlockTypeSupportsBasicCpp
   mkdir build && cd build
   cmake ..
   make
   sudo make install
   ```

4. **Install Block Libraries (e.g., BlockLibrariesBasicBlocksBasicCpp):**  
   Clone and install any desired block library:
   ```bash
   git clone https://github.com/PySysLink/BlockLibrariesBasicBlocksBasicCpp.git
   cd BlockLibrariesBasicBlocksBasicCpp
   mkdir build && cd build
   cmake ..
   make
   sudo make install
   ```

## How the Plug‑in System Works

BlockTypeSupportsBasicCpp provides a dynamic plug‑in mechanism to extend PySysLink with additional block libraries. Its plug‑in loader works as follows:

- **Scanning:**  
  It recursively scans a specified directory (e.g., `/usr/local/lib`) for shared libraries whose filenames match the pattern `"libBlockLibraries*BasicCpp.so"`.

- **Loading:**  
  For each shared library found, the loader uses `dlopen` to load the library into memory.

- **Registration:**  
  It then retrieves the registration functions (whose names are formed by concatenating a fixed prefix such as `"RegisterBasicCppFactories"` with a type-specific suffix—for example, `"Double"` for blocks handling `double` signals or `"Complex"` for `std::complex<double>`) using `dlsym`.  
  These registration functions are called to populate a registry (a map from string keys to block factory instances) with new block factories.

- **Usage:**  
  Once loaded, the new block factories become part of the simulation system and allow PySysLinkBase to create and use blocks from the new block libraries without recompilation.

This plug‑in architecture makes it easy to extend the simulation framework: developers can create new block libraries that adhere to the BlockTypesBasicCpp interfaces, and they simply need to compile them as shared libraries following the naming conventions. BlockTypeSupportsBasicCpp will automatically load these plug‑ins at runtime.
