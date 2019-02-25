using System;
using System.Runtime.Remoting.Channels;
using Ch.Elca.Iiop;
using Ch.Elca.Iiop.Services;
using omg.org.CosNaming;
using hpp.corbaserver;

class Program
{
	static void Main(string[] args)
	{
		Client client = new Client ();

		try
		{
			string nameServiceHost = "localhost";
			int nameServicePort = 2809;
			parseArgs (ref nameServiceHost, ref nameServicePort, args);

			client.connect (nameServiceHost, nameServicePort);
		}
		catch (Exception e)
		{
			Console.WriteLine("Could not connect to the server. The exception is:\n\n" + e);
		}

		bool problemOK = false, robotOK = false, obstacleOK = false;
		// Problem
		try {
			string[] types = client.problem.getAvailable ("type");
			problemOK = (types.Length > 0);
		}
		catch (hpp._Error e) // Catch hpp errors.
		{
			Console.WriteLine("HPP Error: " + e.msg);
		}
		// Robot
		try {
			string name = client.robot.getRobotName ();
			robotOK = true;
		}
		catch (hpp._Error) // Catch hpp errors.
		{
			// May throw if no robot is loaded.
			robotOK = true;
		}
		// Obstacle
		try {
			client.obstacle.getObstacleNames (true, false);
			obstacleOK = true;
		}
		catch (hpp._Error) // Catch hpp errors.
		{
			// May throw if no robot is loaded.
			obstacleOK = true;
		}

		if (robotOK && problemOK && obstacleOK)
			Console.WriteLine ("Everything seems OK.");
	}

	private static void parseArgs(ref string host, ref int port, string[] args)
	{
		for (int i = 0; i < args.Length; ++i) {
			if (args[i] == "--host") {
				++i;
					host = args[i];
			} else if (args[i] == "--port") {
				++i;
				port = Int32.Parse(args[i]);
			}
		}
	}
}
