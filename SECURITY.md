Security Policy for the Quasar Compiler Project
Supported Versions
The Quasar Compiler is an educational project under active development. As such, security patches and updates will only be provided for the latest version available on the main branch of this repository. Due to the rapid and exploratory nature of this project, we do not have the resources to maintain and patch older versions. Contributors and users should always work with the most recent source code to ensure they have the latest security fixes.

| Version | Supported |
| --- | --- |
| 1.0.x | :white_check_mark: |
| < 1.0 | :x: |

Reporting a Vulnerability
The security of this project and the trust of its community are taken seriously. We appreciate any and all efforts to responsibly discover and disclose vulnerabilities.

Please do not report security vulnerabilities through public GitHub issues, pull requests, or any other public forum. Public disclosure of a vulnerability could put other users at risk before a solution is available.

Instead, please report them directly to the project maintainer via email at:
[INSERT YOUR PREFERRED SECURITY CONTACT EMAIL HERE]

To ensure we can effectively address the issue, please include the following information in your report:

A clear and descriptive title for the vulnerability (e.g., "Buffer Overflow in String Literal Parsing").

A detailed description of the vulnerability, its nature, and its potential impact on the compiler or the generated executables.

Clear, step-by-step instructions on how to reproduce the issue. This is the most critical part of the report. It should include a specific input.lang file that triggers the vulnerability and the exact commands used to compile and run the code.

Any relevant information about your environment, such as your operating system (e.g., Windows 11 with MSYS2, Ubuntu 22.04), the version of your build tools (gcc --version), and any other context that might be relevant.

You can expect an acknowledgment of your report within 48 hours. We are committed to working with you to understand, verify, and resolve the issue in a timely manner. We ask that you do not disclose the vulnerability publicly until a patch has been developed and made available.

Security Philosophy
The Quasar Compiler is primarily an educational tool designed to demonstrate the principles of compiler construction from first principles. While correctness and robustness are key goals, the compiler has not been hardened against malicious or intentionally malformed input. Its security scope is limited to its function as a learning project, and it should not be used in a production environment.

We thank you for helping to keep the Quasar Compiler project safe 
