#ifndef certificates_h
#define certificates_h

/*
 * USERTrust RSA Certification Authority 
 * root CA used for Grafana certificates.
 */

static const char grafanaCert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIHkzCCBnugAwIBAgIQDPLuQ6wSK0OVuZKiBlYLmTANBgkqhkiG9w0BAQsFADBZ
MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMTMwMQYDVQQDEypE
aWdpQ2VydCBHbG9iYWwgRzIgVExTIFJTQSBTSEEyNTYgMjAyMCBDQTEwHhcNMjQw
MjAyMDAwMDAwWhcNMjUwMzA0MjM1OTU5WjBhMQswCQYDVQQGEwJVUzERMA8GA1UE
CBMITmV3IFlvcmsxETAPBgNVBAcTCE5ldyBZb3JrMRYwFAYDVQQKEw1SYWludGFu
ayBJbmMuMRQwEgYDVQQDEwtncmFmYW5hLmNvbTCCASIwDQYJKoZIhvcNAQEBBQAD
ggEPADCCAQoCggEBALEJZ5LMSlbgpKr0HqsnlW74+AnSmTbW4sIpBZzSq9scMM2S
XwD6Sckm/eFAEl6Dgt03OZdTlY3Zcx/ksR4yN/vlS5/uMs4/cdXCeJz4gNIkMb2q
yV8E7t8h0KFgTX5sl8y164HguHssMM4wVI5D9ADlh0qigD1e4Rp53HVF/hTnCV9h
gcaEKtQZ4Xv7jXA/k7v06INJVwXSFc6gL8XoGXZpY9IVix3H9RJLI5wZmxQP9AJ9
ysPfuyL2ekeVHv3KDx4RVlFzIXSHMaJmEudfj8erdrYVF8GmBqV31YRWyUvsGDov
cm4hKbikGm8a6KcIYkCQLyH+3UOJd1haX1OK5WMCAwEAAaOCBE0wggRJMB8GA1Ud
IwQYMBaAFHSFgMBmx9833s+9KTeqAx2+7c0XMB0GA1UdDgQWBBSDcwVDNhTPeg+T
Z8OujVeVsg4pxDCB2wYDVR0RBIHTMIHQggtncmFmYW5hLmNvbYINKi5ncmFmYW5h
LmNvbYINKi5ncmFmYW5hLm5ldIINKi5ncmFmYW5hLm9yZ4IcKi5ob3N0ZWQtbWV0
cmljcy5ncmFmYW5hLm5ldIINKi5yYWludGFuay5pb4ILZ3JhZmFuYS5uZXSCC2dy
YWZhbmEub3JnggtyYWludGFuay5pb4IRKi5ncmFmYW5hbGFicy5jb22CD2dyYWZh
bmFsYWJzLmNvbYIOKi5yYWludGFuay5jb22CDHJhaW50YW5rLmNvbTA+BgNVHSAE
NzA1MDMGBmeBDAECAjApMCcGCCsGAQUFBwIBFhtodHRwOi8vd3d3LmRpZ2ljZXJ0
LmNvbS9DUFMwDgYDVR0PAQH/BAQDAgWgMB0GA1UdJQQWMBQGCCsGAQUFBwMBBggr
BgEFBQcDAjCBnwYDVR0fBIGXMIGUMEigRqBEhkJodHRwOi8vY3JsMy5kaWdpY2Vy
dC5jb20vRGlnaUNlcnRHbG9iYWxHMlRMU1JTQVNIQTI1NjIwMjBDQTEtMS5jcmww
SKBGoESGQmh0dHA6Ly9jcmw0LmRpZ2ljZXJ0LmNvbS9EaWdpQ2VydEdsb2JhbEcy
VExTUlNBU0hBMjU2MjAyMENBMS0xLmNybDCBhwYIKwYBBQUHAQEEezB5MCQGCCsG
AQUFBzABhhhodHRwOi8vb2NzcC5kaWdpY2VydC5jb20wUQYIKwYBBQUHMAKGRWh0
dHA6Ly9jYWNlcnRzLmRpZ2ljZXJ0LmNvbS9EaWdpQ2VydEdsb2JhbEcyVExTUlNB
U0hBMjU2MjAyMENBMS0xLmNydDAMBgNVHRMBAf8EAjAAMIIBfgYKKwYBBAHWeQIE
AgSCAW4EggFqAWgAdgBOdaMnXJoQwzhbbNTfP1LrHfDgjhuNacCx+mSxYpo53wAA
AY1qb3rzAAAEAwBHMEUCIQCGpBepB/hi4bgrnwDrecUdWSVOF4Zj57LiF0oprv5k
BgIgZEDooix0RhSVzhiOU4WBfZ9w0Ne9U61zvKq1JhKse/4AdwB9WR4S4Xgqexxh
Z3xe/fjQh1wUoE6VnrkDL9kOjC55uAAAAY1qb3sjAAAEAwBIMEYCIQDhwCO3RkPT
rkNqFBgOrO/IhYLi5QxJr154FYF2J7+V4AIhAIxpIm9tSlftOxEjXMfOU52PdBAi
zNIsn86lU1KsK+4+AHUA5tIxY0B3jMEQQQbXcbnOwdJA9paEhvu6hzId/R43jlAA
AAGNam97TQAABAMARjBEAiB2VsIqnICisW1PU5MM0CqE3pe+T9xUvcAUSC/40dAx
qwIgT0LHqZ1ACkN2YTTyjnQzkIQOmi2E0NlB3VtSuqus2UUwDQYJKoZIhvcNAQEL
BQADggEBADdMnqwxs89lnqVQCTUa4Di393ZHM8IHEU82xF2M9zds1oNH5U6doYC/
g/c0QNbYslMNi4aIVNsJJM+w2mSwZJ2Vh7Yag24R9/M9hh/SkcOXtORRc10u1V+7
MwcVXKq3UTdZbPG+smj1at9LEz5V4B8bjmdYrKVtSlct2qVr2SN+8+5pjNZT51xa
J9ePWqCoClm2Ni+lKl4Lh7xVpr+9sTJRxZi5mVOPUwcsnPWbGtPwycmDMkRTboJK
+MDPTlPOaPC1/iJ61wrecHxm2cFIPmqdromPcPZZUCcQMAJCLigC3Ur9wKHP4605
u3YhLu1VbII5aTPiLXUkindqPDThN/M=
-----END CERTIFICATE-----
)EOF";


#endif