using System;
using System.Runtime.Remoting.Channels;
using Ch.Elca.Iiop;
using Ch.Elca.Iiop.Services;
using omg.org.CosNaming;

namespace hpp
{
    namespace corbaserver
    {
        public class Client
        {
            public Client()
            {
            }

            public void connect(string nameServiceHost)
            {
                connect(nameServiceHost, 2809);
            }

            public void connect(string nameServiceHost, int nameServicePort)
            {
                connect(nameServiceHost, nameServicePort, "");
            }

            //public void connect(string nameServiceHost, int nameServicePort = 2809, string context = "")
            public void connect(string nameServiceHost, int nameServicePort, string context)
            {
                channel = new IiopClientChannel();
                ChannelServices.RegisterChannel(channel, false);

                // access OmniORB Name Service
                CorbaInit init = CorbaInit.GetInit();
                NamingContext nameService = init.GetNameService(nameServiceHost, nameServicePort);

                NameComponent[] nRobot = new NameComponent[] { new NameComponent("hpp"+context, "corbaserver"), new NameComponent("basic", "robot") };
                robot = (hpp.corbaserver.RobotIDL)nameService.resolve(nRobot);
                NameComponent[] nProblem = new NameComponent[] { new NameComponent("hpp" + context, "corbaserver"), new NameComponent("basic", "problem") };
                problem = (hpp.corbaserver.Problem)nameService.resolve(nProblem);
                NameComponent[] nObstacle = new NameComponent[] { new NameComponent("hpp" + context, "corbaserver"), new NameComponent("basic", "obstacle") };
                obstacle = (hpp.corbaserver.Obstacle)nameService.resolve(nObstacle);
            }

            private IiopClientChannel channel;

            public hpp.corbaserver.RobotIDL robot;
            public hpp.corbaserver.Problem  problem;
            public hpp.corbaserver.Obstacle obstacle;
        };

    }
}