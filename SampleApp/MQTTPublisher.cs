using System;
using System.Linq;
using System.Net.Sockets;
using System.Runtime.InteropServices;

namespace SampleApp;

internal class MQTTPublisher : IDisposable
{
    private readonly TcpClient tcpClient;
    private bool _disposed;

    public MQTTPublisher()
    {
        tcpClient = new TcpClient();
    }

    protected virtual void Dispose(bool disposing)
    {
        if (!_disposed)
        {
            if (disposing)
            {
                using (tcpClient) {}
            }

            _disposed = true;
        }
    }

    ~MQTTPublisher()
    {
        Dispose(disposing: false);
    }

    public void Dispose()
    {
        Dispose(disposing: true);
        GC.SuppressFinalize(this);
    }

    public bool Connect(string hostname, int port)
    {
        byte[] password = "readonly".ToCharArray().Select(c => (byte)c).ToArray();
        IntPtr pPassword = Marshal.AllocCoTaskMem(Marshal.SizeOf(typeof(char)) * password.Length);
        Marshal.Copy(password, 0, pPassword, password.Length);

        var option = new NativeMethods.MQTT_ConnectPacketOption()
        {
            pszClientID = "SampleApp",
            pszUserName = "ro",
            pPassword = pPassword,
            wPasswordLength = (ushort)password.Length,
            wKeepAlive = 0,
            bCleanStart = 0,
        };

        tcpClient.Connect(hostname, port);
        if (!tcpClient.Connected)
        {
            return false;
        }

        var dwBufferLength = 1024U;
        var pBuffer = new Byte[dwBufferLength];

        int packetLength = NativeMethods.MQTT_CreateConnectPacket(pBuffer, dwBufferLength, ref option);
        if (packetLength <= 0)
        {
            tcpClient.Close();
            return false;
        }

        tcpClient.GetStream().Write(pBuffer, 0, packetLength);

        return true;
    }

    private static class NativeMethods
    {
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
        internal struct MQTT_ConnectPacketOption
        {
            /* Null terminated utf-8 string (Recommend less than 23 bytes). */
            [MarshalAs(UnmanagedType.LPUTF8Str)]
            public string pszClientID;

            /* Null terminated utf-8 string. Empty string is also valid. */
            [MarshalAs(UnmanagedType.LPUTF8Str)]
            public string? pszUserName;

            /* 0 to 65535 Bytes binary data. */
            public IntPtr pPassword;
            public ushort wPasswordLength;

            /* A time interval measured in seconds. 0 effects turning off this mechanism. */
            public ushort wKeepAlive;

            /* Boolean value (0 or 1). */
            public byte bCleanStart;

            /* TODO: Support Will Flag */
            /* TODO: Support Properties */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            private byte[] reserved;

            public MQTT_ConnectPacketOption()
            {
                pszClientID = string.Empty;
                pszUserName = null;
                pPassword = IntPtr.Zero;
                wPasswordLength = 0;
                wKeepAlive = 0;
                bCleanStart = 0;
                reserved = new byte[3];
            }
        }

        [DllImport("SimpleMQTTPublisher.dll")]
        internal static extern int MQTT_CreateConnectPacket(Byte[] pBuffer, uint dwBufferLength, ref MQTT_ConnectPacketOption pOption);
    }
}
