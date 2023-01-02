Sample init scripts and service configuration for sumcoind
==========================================================

Sample scripts and configuration files for systemd, Upstart and OpenRC
can be found in the contrib/init folder.

    contrib/init/sumcoind.service:    systemd service unit configuration
    contrib/init/sumcoind.openrc:     OpenRC compatible SysV style init script
    contrib/init/sumcoind.openrcconf: OpenRC conf.d file
    contrib/init/sumcoind.conf:       Upstart service configuration file
    contrib/init/sumcoind.init:       CentOS compatible SysV style init script

Service User
---------------------------------

All three Linux startup configurations assume the existence of a "sumcoin" user
and group.  They must be created before attempting to use these scripts.
The macOS configuration assumes sumcoind will be set up for the current user.

Configuration
---------------------------------

At a bare minimum, sumcoind requires that the rpcpassword setting be set
when running as a daemon.  If the configuration file does not exist or this
setting is not set, sumcoind will shut down promptly after startup.

This password does not have to be remembered or typed as it is mostly used
as a fixed token that sumcoind and client programs read from the configuration
file, however it is recommended that a strong and secure password be used
as this password is security critical to securing the wallet should the
wallet be enabled.

If sumcoind is run with the "-server" flag (set by default), and no rpcpassword is set,
it will use a special cookie file for authentication. The cookie is generated with random
content when the daemon starts, and deleted when it exits. Read access to this file
controls who can access it through RPC.

By default the cookie is stored in the data directory, but it's location can be overridden
with the option '-rpccookiefile'.

This allows for running sumcoind without having to do any manual configuration.

`conf`, `pid`, and `wallet` accept relative paths which are interpreted as
relative to the data directory. `wallet` *only* supports relative paths.

For an example configuration file that describes the configuration settings,
see `share/examples/sumcoin.conf`.

Paths
---------------------------------

### Linux

All three configurations assume several paths that might need to be adjusted.

Binary:              `/usr/bin/sumcoind`
Configuration file:  `/etc/sumcoin/sumcoin.conf`
Data directory:      `/var/lib/sumcoind`
PID file:            `/var/run/sumcoind/sumcoind.pid` (OpenRC and Upstart) or `/run/sumcoind/sumcoind.pid` (systemd)  
Lock file:           `/var/lock/subsys/sumcoind` (CentOS)

The PID directory (if applicable) and data directory should both be owned by the
sumcoin user and group. It is advised for security reasons to make the
configuration file and data directory only readable by the sumcoin user and
group. Access to sumcoin-cli and other sumcoind rpc clients can then be
controlled by group membership.

NOTE: When using the systemd .service file, the creation of the aforementioned
directories and the setting of their permissions is automatically handled by
systemd. Directories are given a permission of 710, giving the bitcoin group
access to files under it _if_ the files themselves give permission to the
bitcoin group to do so (e.g. when `-sysperms` is specified). This does not allow
for the listing of files under the directory.

NOTE: It is not currently possible to override `datadir` in
`/etc/bitcoin/bitcoin.conf` with the current systemd, OpenRC, and Upstart init
files out-of-the-box. This is because the command line options specified in the
init files take precedence over the configurations in
`/etc/bitcoin/bitcoin.conf`. However, some init systems have their own
configuration mechanisms that would allow for overriding the command line
options specified in the init files (e.g. setting `BITCOIND_DATADIR` for
OpenRC).

### macOS

NOTE: It is not currently possible to override `datadir` in
`/etc/sumcoin/sumcoin.conf` with the current systemd, OpenRC, and Upstart init
files out-of-the-box. This is because the command line options specified in the
init files take precedence over the configurations in
`/etc/sumcoin/sumcoin.conf`. However, some init systems have their own
configuration mechanisms that would allow for overriding the command line
options specified in the init files (e.g. setting `BITCOIND_DATADIR` for
OpenRC).

### macOS

Binary:              `/usr/local/bin/sumcoind`
Configuration file:  `~/Library/Application Support/Sumcoin/sumcoin.conf`
Data directory:      `~/Library/Application Support/Sumcoin`
Lock file:           `~/Library/Application Support/Sumcoin/.lock`

Installing Service Configuration
-----------------------------------

### systemd

Installing this .service file consists of just copying it to
/usr/lib/systemd/system directory, followed by the command
`systemctl daemon-reload` in order to update running systemd configuration.

To test, run `systemctl start sumcoind` and to enable for system startup run
`systemctl enable sumcoind`

NOTE: When installing for systemd in Debian/Ubuntu the .service file needs to be copied to the /lib/systemd/system directory instead.

### OpenRC

Rename sumcoind.openrc to sumcoind and drop it in /etc/init.d.  Double
check ownership and permissions and make it executable.  Test it with
`/etc/init.d/sumcoind start` and configure it to run on startup with
`rc-update add sumcoind`

### Upstart (for Debian/Ubuntu based distributions)

Upstart is the default init system for Debian/Ubuntu versions older than 15.04. If you are using version 15.04 or newer and haven't manually configured upstart you should follow the systemd instructions instead.

Drop sumcoind.conf in /etc/init.  Test by running `service sumcoind start`
it will automatically start on reboot.

NOTE: This script is incompatible with CentOS 5 and Amazon Linux 2014 as they
use old versions of Upstart and do not supply the start-stop-daemon utility.

### CentOS

Copy sumcoind.init to /etc/init.d/sumcoind. Test by running `service sumcoind start`.

Using this script, you can adjust the path and flags to the sumcoind program by
setting the SUMCASHD and FLAGS environment variables in the file
/etc/sysconfig/sumcoind. You can also use the DAEMONOPTS environment variable here.

### macOS

Copy org.sumcoin.sumcoind.plist into ~/Library/LaunchAgents. Load the launch agent by
running `launchctl load ~/Library/LaunchAgents/org.sumcoin.sumcoind.plist`.

This Launch Agent will cause sumcoind to start whenever the user logs in.

NOTE: This approach is intended for those wanting to run sumcoind as the current user.
You will need to modify org.sumcoin.sumcoind.plist if you intend to use it as a
Launch Daemon with a dedicated sumcoin user.

Auto-respawn
-----------------------------------

Auto respawning is currently only configured for Upstart and systemd.
Reasonable defaults have been chosen but YMMV.
