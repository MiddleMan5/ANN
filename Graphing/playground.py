
import ns.applications
import ns.bridge
import ns.core
import ns.csma
import ns.internet
import ns.network
import ns.mobility
import ns.visualizer
import ns.flow_monitor




def main(argv):
    cmd = ns.core.CommandLine()
    cmd.NumNodesSide = True
    cmd.AddValue("NumNodesSide", "Grid side number of nodes (total number of nodes will be this number squared)")

    cmd.Results = None
    cmd.AddValue("Results", "Write XML results to file")

    cmd.Plot = True
    cmd.AddValue("Plot", "Plot the results using the matplotlib python module")
    cmd.Parse(argv)

    #print ("Crea nodos")
    terminals = ns.network.NodeContainer()
    terminals.Create(5)

    csmaSwitch = ns.network.NodeContainer()
    csmaSwitch.Create(1)

    csmaSwitch2 = ns.network.NodeContainer()
    csmaSwitch2.Create(1)

    #print "Construye la topologia"
    csma = ns.csma.CsmaHelper()
    csma.SetChannelAttribute("DataRate", ns.network.DataRateValue(ns.network.DataRate(5000000)))
    csma.SetChannelAttribute("Delay", ns.core.TimeValue(ns.core.MilliSeconds(2)))

    # Crea los  links de cada terminal al switch
    terminalDevices = ns.network.NetDeviceContainer()
    switchDevices = ns.network.NetDeviceContainer()
    switchDevices2 = ns.network.NetDeviceContainer()

    #se unen los cuatro nodos al primer switch
    for i in range(4):
        link = csma.Install(ns.network.NodeContainer(ns.network.NodeContainer(terminals.Get(i)), csmaSwitch))
        terminalDevices.Add(link.Get(0))
        switchDevices.Add(link.Get(1))
    #se une un nodo al segundo switch
    link = csma.Install(ns.network.NodeContainer(ns.network.NodeContainer(terminals.Get(2)), csmaSwitch2))
    terminalDevices.Add(link.Get(0))
    switchDevices2.Add(link.Get(1))
    link = csma.Install(ns.network.NodeContainer(ns.network.NodeContainer(terminals.Get(4)), csmaSwitch2))
    terminalDevices.Add(link.Get(0))
    switchDevices2.Add(link.Get(1))

    #se crean los puentes para enviar los paquetes
    switchNode = csmaSwitch.Get(0)
    bridgeDevice = ns.bridge.BridgeNetDevice()
    switchNode.AddDevice(bridgeDevice)

    switchNode2 = csmaSwitch2.Get(0)
    bridgeDevice2 = ns.bridge.BridgeNetDevice()
    switchNode2.AddDevice(bridgeDevice2)

    for portIter in range(switchDevices.GetN()):
        bridgeDevice.AddBridgePort(switchDevices.Get(portIter))
    for portIter in range(switchDevices2.GetN()):
        bridgeDevice2.AddBridgePort(switchDevices2.Get(portIter))

    # se agrega el internet a las terminales
    internet = ns.internet.InternetStackHelper()
    internet.Install(terminals)

    # Se agregan las direcciones iP
    #print "Se asignan direcciones IP"
    ipv4 = ns.internet.Ipv4AddressHelper()
    ipv4.SetBase(ns.network.Ipv4Address("10.1.1.0"), ns.network.Ipv4Mask("255.255.255.0"))
    ipv4.Assign(terminalDevices)

    #para dibujar los nodos
    mobility = ns.mobility.ConstantPositionMobilityModel()
    mobility.SetPosition(ns.core.Vector(95, 5, 0))
    terminals.Get(0).AggregateObject(mobility)

    mobility = ns.mobility.ConstantPositionMobilityModel()
    mobility.SetPosition(ns.core.Vector(120,35, 0))
    terminals.Get(1).AggregateObject(mobility)

    mobility = ns.mobility.ConstantPositionMobilityModel()
    mobility.SetPosition(ns.core.Vector(60,35, 0))
    terminals.Get(2).AggregateObject(mobility)

    mobility = ns.mobility.ConstantPositionMobilityModel()
    mobility.SetPosition(ns.core.Vector(100,65, 0))
    terminals.Get(3).AggregateObject(mobility)

    mobility = ns.mobility.ConstantPositionMobilityModel()
    mobility.SetPosition(ns.core.Vector(10,35, 0))
    terminals.Get(4).AggregateObject(mobility)

    mobility1 = ns.mobility.ConstantPositionMobilityModel()
    mobility1.SetPosition(ns.core.Vector(90,45, 0))
    csmaSwitch.Get(0).AggregateObject(mobility1)

    mobility2 = ns.mobility.ConstantPositionMobilityModel()
    mobility2.SetPosition(ns.core.Vector(40,40, 0))
    csmaSwitch2.Get(0).AggregateObject(mobility2)

    # se crea una aplicacion para enviar paquetes UDP del nodo cero al nodo 1.
    #print "Create Applications."
    port = 9
    onoff = ns.applications.OnOffHelper("ns3::UdpSocketFactory",
                            ns.network.Address(ns.network.InetSocketAddress(ns.network.Ipv4Address("10.1.1.2"), port)))
    onoff.SetConstantRate (ns.network.DataRate ("500kb/s"))
    app = onoff.Install(ns.network.NodeContainer(terminals.Get(0)))
    # empieza la aplicacion
    app.Start(ns.core.Seconds(1.0))
    sink = ns.applications.PacketSinkHelper("ns3::UdpSocketFactory",
                                ns.network.Address(ns.network.InetSocketAddress(ns.network.Ipv4Address.GetAny(), port)))
    app = sink.Install(ns.network.NodeContainer(terminals.Get(1)))
    app.Start(ns.core.Seconds(0.0))

    # se envian paquetes del nodo 3 al 0
    onoff.SetConstantRate (ns.network.DataRate ("100kb/s"))
    onoff.SetAttribute("Remote",
                       ns.network.AddressValue(ns.network.InetSocketAddress(ns.network.Ipv4Address("10.1.1.1"), port)))
    app = onoff.Install(ns.network.NodeContainer(terminals.Get(3)))
    app.Start(ns.core.Seconds(1.1))
    app = sink.Install(ns.network.NodeContainer(terminals.Get(0)))
    app.Start(ns.core.Seconds(2.0))

    #se envian paquetes del nodo 2 al 4
    onoff.SetConstantRate (ns.network.DataRate ("300kb/s"))
    onoff.SetAttribute("Remote",
                       ns.network.AddressValue(ns.network.InetSocketAddress(ns.network.Ipv4Address("10.1.1.6"), port)))
    app = onoff.Install(ns.network.NodeContainer(terminals.Get(2)))
    app.Start(ns.core.Seconds(1.1))
    app = sink.Install(ns.network.NodeContainer(terminals.Get(4)))
    app.Start(ns.core.Seconds(0.0))

    #se envian paquetes del nodo 1 al 2
    onoff.SetConstantRate (ns.network.DataRate ("160kb/s"))
    onoff.SetAttribute("Remote",
                       ns.network.AddressValue(ns.network.InetSocketAddress(ns.network.Ipv4Address("10.1.1.3"), port)))
    app = onoff.Install(ns.network.NodeContainer(terminals.Get(1)))
    app.Start(ns.core.Seconds(1.1))
    app = sink.Install(ns.network.NodeContainer(terminals.Get(2)))
    app.Start(ns.core.Seconds(0.0))
    csma.EnablePcapAll("csma-bridge", False)

    #se envian paquetes del nodo 3 al 2
    onoff.SetConstantRate (ns.network.DataRate ("800kb/s"))
    onoff.SetAttribute("Remote",
                       ns.network.AddressValue(ns.network.InetSocketAddress(ns.network.Ipv4Address("10.1.1.3"), port)))
    app = onoff.Install(ns.network.NodeContainer(terminals.Get(3)))
    app.Start(ns.core.Seconds(1.1))
    app = sink.Install(ns.network.NodeContainer(terminals.Get(2)))
    app.Start(ns.core.Seconds(0.0))
    csma.EnablePcapAll("csma-bridge", False)

    #se envian paquetes del 4 al 2
    onoff.SetAttribute("Remote",
                       ns.network.AddressValue(ns.network.InetSocketAddress(ns.network.Ipv4Address("10.1.1.5"), port)))
    app = onoff.Install(ns.network.NodeContainer(terminals.Get(4)))
    app.Start(ns.core.Seconds(1.1))
    app = sink.Install(ns.network.NodeContainer(terminals.Get(2)))
    app.Start(ns.core.Seconds(0.0))


    flowmon_helper = ns.flow_monitor.FlowMonitorHelper()
    monitor = flowmon_helper.InstallAll()
    monitor = flowmon_helper.GetMonitor()
    monitor.SetAttribute("DelayBinWidth", ns.core.DoubleValue(0.1))
    monitor.SetAttribute("JitterBinWidth", ns.core.DoubleValue(0.1))
    monitor.SetAttribute("PacketSizeBinWidth", ns.core.DoubleValue(20))
    #print "Empieza la simulacion"
    ns.core.Simulator.Run()
    ns.core.Simulator.Destroy()

    def #print_stats(os, st):
        #print  >> os, "  Tx Bytes: ", st.txBytes
        #print  >> os, "  Rx Bytes: ", st.rxBytes
        #print  >> os, "  Tx Packets: ", st.txPackets
        #print  >> os, "  Rx Packets: ", st.rxPackets
        #print  >> os, "  Lost Packets: ", st.lostPackets
        if st.rxPackets > 0:
            #print >> os, "  Mean{Delay}: ", (st.delaySum.GetSeconds() / st.rxPackets)
            #print >> os, "  Mean{Jitter}: ", (st.jitterSum.GetSeconds() / (st.rxPackets-1))
            #print >> os, "  Mean{Hop Count}: ", float(st.timesForwarded) / st.rxPackets + 1

        if 0:
            #print >> os, "Delay Histogram"
            for i in range(st.delayHistogram.GetNBins () ):
              #print >> os, " ",i,"(", st.delayHistogram.GetBinStart (i), "-", \
                  st.delayHistogram.GetBinEnd (i), "): ", st.delayHistogram.GetBinCount (i)
            #print >> os, "Jitter Histogram"
            for i in range(st.jitterHistogram.GetNBins () ):
              #print >> os, " ",i,"(", st.jitterHistogram.GetBinStart (i), "-", \
                  st.jitterHistogram.GetBinEnd (i), "): ", st.jitterHistogram.GetBinCount (i)
            #print >> os, "PacketSize Histogram"
            for i in range(st.packetSizeHistogram.GetNBins () ):
              #print >> os, " ",i,"(", st.packetSizeHistogram.GetBinStart (i), "-", \
                  st.packetSizeHistogram.GetBinEnd (i), "): ", st.packetSizeHistogram.GetBinCount (i)

        for reason, drops in enumerate(st.packetsDropped):
            #print "  Packets dropped by reason %i: %i" % (reason, drops)

    monitor.CheckForLostPackets()
    classifier = flowmon_helper.GetClassifier()

    if cmd.Results is None:
        for flow_id, flow_stats in monitor.GetFlowStats():
            t = classifier.FindFlow(flow_id)
            proto = {6: 'TCP', 17: 'UDP'} [t.protocol]
            #print "FlowID: %i (%s %s/%s --> %s/%i)" % \
                (flow_id, proto, t.sourceAddress, t.sourcePort, t.destinationAddress, t.destinationPort)
            #print_stats(sys.stdout, flow_stats)
    else:
        #print monitor.SerializeToXmlFile(cmd.Results, True, True)
    if cmd.Plot is not None:
        import pylab
        delays = []
        for flow_id, flow_stats in monitor.GetFlowStats():
            tupl = classifier.FindFlow(flow_id)
            if tupl.protocol == 17 and tupl.sourcePort == 698:
                continue
            delays.append(flow_stats.delaySum.GetSeconds() / flow_stats.rxPackets)
        pylab.hist(delays, 20)
        pylab.xlabel("Delay (s)")
        pylab.ylabel("Number of Flows")
        pylab.show()

    #print "Finaliza"


if __name__ == '__main__':
    import sys
    main(sys.argv)
