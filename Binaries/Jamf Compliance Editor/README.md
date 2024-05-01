# Jamf Compliance Editor Exploit Mirror

See [CVE-2024-4395: Jamf Compliance Editor Privilege Escalation](https://khronokernel.com/macos/2024/05/01/CVE-2024-4395.html) for write up.

* Proof of Concept:
  * [jce_exploit.m](jce_exploit.m)
  * [Exploit Demo](Exploit%20Demo.mov)
* Jamf Compliance Editor v1.3:
  * [Jamf Compliance Editor v1.3.tar.aa](Jamf%20Compliance%20Editor%20v1.3.tar.aa)
  * [Jamf Compliance Editor v1.3.tar.ab](Jamf%20Compliance%20Editor%20v1.3.tar.ab)
  * [Jamf Compliance Editor v1.3.tar.ac](Jamf%20Compliance%20Editor%20v1.3.tar.ac)
* Jamf Compliance Editor v1.3.1:
  * [Jamf Compliance Editor v1.3.1.pkg](Jamf%20Compliance%20Editor%20v1.3.1.pkg)


## Jamf Compliance Editor v1.3 reassembly

Due to GitHub's 100MB limit, JCE v1.3 is split into multiple files. To reassemble the binary, run the following command in Terminal:

```bash
/bin/cat "Jamf Compliance Editor v1.3.tar."* > "Jamf Compliance Editor v1.3.tar"
```

To replicate the split, run the following command in Terminal:

```bash
/usr/bin/split -b 50m "Jamf Compliance Editor v1.3.tar" "Jamf Compliance Editor v1.3.tar."
```