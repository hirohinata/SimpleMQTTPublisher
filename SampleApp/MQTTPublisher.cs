using System;
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
        tcpClient.Connect(hostname, port);
        if (!tcpClient.Connected)
        {
            return false;
        }

        var dwBufferLength = 1024U;
        var pBuffer = new Byte[dwBufferLength];

        int packetLength = NativeMethods.MQTT_CreateConnectPacket(pBuffer, dwBufferLength);
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
        [DllImport("SimpleMQTTPublisher.dll")]
        internal static extern int MQTT_CreateConnectPacket(Byte[] pBuffer, uint dwBufferLength);
    }
}
