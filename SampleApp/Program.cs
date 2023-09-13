using System;
using System.Net.Sockets;

namespace SampleApp;

static class Program
{
    static void Main(string[] args)
    {
        try
        {
            System.IO.File.Copy("../../../../x64/Debug/SimpleMQTTPublisher.dll", "SimpleMQTTPublisher.dll", true);
            System.IO.File.Copy("../../../../x64/Debug/SimpleMQTTPublisher.pdb", "SimpleMQTTPublisher.pdb", true);

            using var client = new MQTTPublisher();
            client.Connect("test.mosquitto.org", 1884);
        }
        catch (ArgumentNullException e)
        {
            Console.WriteLine("ArgumentNullException: {0}", e);
        }
        catch (SocketException e)
        {
            Console.WriteLine("SocketException: {0}", e);
        }
    }
}
