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

                NameComponent hppName = new NameComponent("hpp" + context, "corbaserver");
                robot    = (RobotIDL)nameService.resolve(new NameComponent[] { hppName, new NameComponent("basic", "robot"   ) });
                problem  = (Problem) nameService.resolve(new NameComponent[] { hppName, new NameComponent("basic", "problem" ) });
                obstacle = (Obstacle)nameService.resolve(new NameComponent[] { hppName, new NameComponent("basic", "obstacle") });
            }

            private IiopClientChannel channel;

            public RobotIDL robot;
            public Problem problem;
            public Obstacle obstacle;
        };

    }
}
