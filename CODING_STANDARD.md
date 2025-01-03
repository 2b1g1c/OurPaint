# Coding Standards

1. **Use CamelCase for Naming**
   - All identifiers, including variables, functions, classes, and methods, should follow CamelCase conventions.
   - *Example:*
     ```cpp
     int userAge;
     void calculateTotal();
     class UserProfile {};
     ```

2. **Icons Only in PNG or SVG Format**
   - All icon assets must be in either PNG or SVG formats to ensure compatibility and quality.

3. **Library Additions Require Discussion by at Least Three Contributors**
   - Any proposal to include new libraries must be reviewed and discussed by a minimum of three contributors before integration.

4. **Comments Must Be in English Only**
   - All code comments should be written in English to maintain consistency and ensure understandability for all team members.

5. **Do Not Rename Functions, Classes, or Files Without Comprehensive Dependency Analysis**
   - Renaming of functions, classes, or files is prohibited unless a thorough analysis of all dependencies is conducted to ensure the changes do not disrupt functionality.
   - *Guidelines:*
     - Utilize refactoring tools to safely rename identifiers.
     - Perform extensive testing after renaming to verify correct behavior.
     - Update all related documentation and references accordingly.

6. **File Names with `.h` and `.cpp` Extensions Must Start with a Capital Letter and Use CamelCase**
   - Ensure that all header (`.h`) and source (`.cpp`) file names begin with an uppercase letter and follow CamelCase formatting.
   - *Examples:*
     - `UserProfile.h`
     - `UserProfile.cpp`
