1) used exploit/windows/smb/ms17_010_eternalblue for shell

2) mimikatz can run cuz Credential Guard disable and RunAsPPL and credential obtaining)

	(PrivescCheck output)

	+-----------------------------------------------------------------------------+
	|                         ~~~ PrivescCheck Report ~~~                         |
	+----+------+-----------------------------------------------------------------+
	| OK | None | CONFIG > WSUS Configuration                                     |
	| OK | None | CONFIG > PrintNightmare exploit                                 |
	| NA | None | CONFIG > SCCM Cache Folder (info)                               |
	| NA | None | CREDS > Vault Creds                                             |
	| OK | None | CREDS > GPP Passwords                                           |
	| OK | None | CREDS > WinLogon                                                |
	| NA | None | CREDS > Vault List                                              |
	| OK | None | CREDS > Unattend Files                                          |
	| NA | Info | HARDENING > Credential Guard -> 1 result(s)                     |
	| NA | Info | HARDENING > BitLocker -> 1 result(s)                            |
	| NA | Info | HARDENING > LSA Protection (RunAsPPL) -> 1 result(s)            |
	| NA | None | SERVICES > Non-default Services                                 |
	| KO | Med. | UPDATES > System up to date? -> 1 result(s)                     |
	| NA | Info | USER > Identity -> 1 result(s)                                  |
	| NA | Info | USER > Groups -> 11 result(s)                                   |
	| NA | None | USER > Environment Variables                                    |
	+----+------+-----------------------------------------------------------------+

3) used Named Pipe Impersonation for PE

4) Credential gathering with mimikatz (running as admin) 
