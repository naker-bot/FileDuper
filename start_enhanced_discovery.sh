#!/bin/bash

echo "🔍 ENIGMACHAT - VERBESSERTE PEER-DISCOVERY"
echo "=========================================="

cat > /home/nex/c++/enigmachat_enhanced_discovery.java << 'EOF'
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.net.*;
import java.io.*;
import java.util.*;
import java.util.List;
import java.util.concurrent.*;

public class enigmachat_enhanced_discovery extends JFrame {
    private JTextArea messageArea;
    private JTextField messageInput;
    private JList<String> peerList;
    private DefaultListModel<String> peerModel;
    private String selectedPeer = null;
    private ServerSocket serverSocket;
    private boolean isServerRunning = false;
    private ExecutorService executor;
    private Set<String> knownPeers;
    private int localPort = 8888;
    
    public enigmachat_enhanced_discovery() {
        knownPeers = new HashSet<>();
        executor = Executors.newFixedThreadPool(20);
        initGUI();
        startEnigmaChatServer();
        startMultipleDiscoveryMethods();
        showInstructions();
    }
    
    private void initGUI() {
        setTitle("🔍 EnigmaChat - Erweiterte Peer-Discovery");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(1000, 750);
        setLocationRelativeTo(null);
        setLayout(new BorderLayout());
        
        // Chat-Bereich
        messageArea = new JTextArea();
        messageArea.setEditable(false);
        messageArea.setBackground(new Color(25, 25, 25));
        messageArea.setForeground(Color.GREEN);
        messageArea.setFont(new Font("Monospace", Font.PLAIN, 11));
        messageArea.setText("🔍 EnigmaChat - Erweiterte Peer-Discovery gestartet!\n");
        messageArea.append("🌐 Verschiedene Discovery-Methoden werden getestet...\n\n");
        
        JScrollPane scrollPane = new JScrollPane(messageArea);
        add(scrollPane, BorderLayout.CENTER);
        
        // Peer-Liste
        peerModel = new DefaultListModel<>();
        peerList = new JList<>(peerModel);
        peerList.setBackground(new Color(40, 40, 40));
        peerList.setForeground(Color.WHITE);
        peerList.setSelectionBackground(new Color(0, 123, 255));
        peerList.setFont(new Font("Monospace", Font.PLAIN, 10));
        
        peerList.addListSelectionListener(e -> {
            if (!e.getValueIsAdjusting()) {
                selectedPeer = peerList.getSelectedValue();
                if (selectedPeer != null) {
                    messageArea.append("✅ Peer ausgewählt: " + selectedPeer + "\n\n");
                    messageArea.setCaretPosition(messageArea.getDocument().getLength());
                }
            }
        });
        
        JPanel leftPanel = new JPanel(new BorderLayout());
        leftPanel.setBackground(new Color(40, 40, 40));
        
        JLabel peerLabel = new JLabel("🌐 Gefundene EnigmaChat-Peers:");
        peerLabel.setForeground(Color.WHITE);
        peerLabel.setBorder(BorderFactory.createEmptyBorder(5, 5, 5, 5));
        
        leftPanel.add(peerLabel, BorderLayout.NORTH);
        leftPanel.add(new JScrollPane(peerList), BorderLayout.CENTER);
        leftPanel.setPreferredSize(new Dimension(320, 0));
        add(leftPanel, BorderLayout.WEST);
        
        // Input und Control Panel
        JPanel bottomPanel = new JPanel(new BorderLayout());
        
        // Input-Bereich
        JPanel inputPanel = new JPanel(new BorderLayout());
        messageInput = new JTextField();
        messageInput.setBackground(new Color(60, 60, 60));
        messageInput.setForeground(Color.WHITE);
        messageInput.setFont(new Font("Arial", Font.PLAIN, 12));
        messageInput.setBorder(BorderFactory.createEmptyBorder(8, 10, 8, 10));
        
        JButton sendButton = new JButton("📤 Nachricht senden");
        sendButton.setBackground(new Color(0, 123, 255));
        sendButton.setForeground(Color.WHITE);
        sendButton.setFont(new Font("Arial", Font.BOLD, 11));
        
        sendButton.addActionListener(e -> sendMessage());
        messageInput.addActionListener(e -> sendMessage());
        
        inputPanel.add(new JLabel("💬 Nachricht: "), BorderLayout.WEST);
        inputPanel.add(messageInput, BorderLayout.CENTER);
        inputPanel.add(sendButton, BorderLayout.EAST);
        
        // Control-Buttons
        JPanel controlPanel = new JPanel(new FlowLayout());
        controlPanel.setBackground(new Color(50, 50, 50));
        
        JButton refreshButton = createButton("🔄 Suche starten", new Color(40, 167, 69));
        JButton broadcastButton = createButton("📡 Broadcast senden", new Color(255, 193, 7));
        JButton manualButton = createButton("➕ IP manuell hinzufügen", new Color(220, 53, 69));
        JButton statusButton = createButton("📊 Status anzeigen", new Color(108, 117, 125));
        
        refreshButton.addActionListener(e -> startMultipleDiscoveryMethods());
        broadcastButton.addActionListener(e -> sendBroadcast());
        manualButton.addActionListener(e -> addManualPeer());
        statusButton.addActionListener(e -> showStatus());
        
        controlPanel.add(refreshButton);
        controlPanel.add(broadcastButton);
        controlPanel.add(manualButton);
        controlPanel.add(statusButton);
        
        bottomPanel.add(inputPanel, BorderLayout.NORTH);
        bottomPanel.add(controlPanel, BorderLayout.SOUTH);
        add(bottomPanel, BorderLayout.SOUTH);
        
        // Status-Panel
        JPanel statusPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        statusPanel.setBackground(new Color(33, 37, 41));
        
        JLabel statusLabel = new JLabel("🟢 EnigmaChat Server läuft auf Port " + localPort);
        statusLabel.setForeground(Color.WHITE);
        statusLabel.setFont(new Font("Arial", Font.BOLD, 12));
        
        statusPanel.add(statusLabel);
        add(statusPanel, BorderLayout.NORTH);
    }
    
    private JButton createButton(String text, Color color) {
        JButton button = new JButton(text);
        button.setBackground(color);
        button.setForeground(Color.WHITE);
        button.setFont(new Font("Arial", Font.BOLD, 10));
        button.setBorder(BorderFactory.createEmptyBorder(6, 12, 6, 12));
        button.setFocusPainted(false);
        return button;
    }
    
    private void startEnigmaChatServer() {
        executor.submit(() -> {
            try {
                serverSocket = new ServerSocket(localPort);
                isServerRunning = true;
                
                SwingUtilities.invokeLater(() -> {
                    messageArea.append("🟢 EnigmaChat Server gestartet auf Port " + localPort + "\n");
                    messageArea.append("📡 Warte auf eingehende Verbindungen...\n\n");
                    messageArea.setCaretPosition(messageArea.getDocument().getLength());
                });
                
                while (isServerRunning) {
                    try {
                        Socket clientSocket = serverSocket.accept();
                        handleIncomingConnection(clientSocket);
                    } catch (Exception e) {
                        if (isServerRunning) {
                            SwingUtilities.invokeLater(() -> {
                                messageArea.append("⚠️ Server-Fehler: " + e.getMessage() + "\n");
                                messageArea.setCaretPosition(messageArea.getDocument().getLength());
                            });
                        }
                    }
                }
            } catch (Exception e) {
                SwingUtilities.invokeLater(() -> {
                    messageArea.append("❌ Server konnte nicht gestartet werden: " + e.getMessage() + "\n");
                    messageArea.setCaretPosition(messageArea.getDocument().getLength());
                });
            }
        });
    }
    
    private void handleIncomingConnection(Socket socket) {
        executor.submit(() -> {
            try {
                BufferedReader reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                String message = reader.readLine();
                
                String clientIP = socket.getInetAddress().getHostAddress();
                
                SwingUtilities.invokeLater(() -> {
                    messageArea.append("📥 Nachricht von " + clientIP + ":\n");
                    messageArea.append("   💬 " + message + "\n\n");
                    
                    // Peer zur Liste hinzufügen
                    String peerInfo = "🟢 EnigmaChat-Peer (" + clientIP + ":" + localPort + ")";
                    if (knownPeers.add(peerInfo)) {
                        peerModel.addElement(peerInfo);
                        messageArea.append("✅ Neuer Peer entdeckt: " + clientIP + "\n\n");
                    }
                    
                    messageArea.setCaretPosition(messageArea.getDocument().getLength());
                });
                
                socket.close();
            } catch (Exception e) {
                // Verbindung geschlossen
            }
        });
    }
    
    private void startMultipleDiscoveryMethods() {
        messageArea.append("🔍 Starte erweiterte Peer-Discovery...\n");
        messageArea.append("🌐 Teste verschiedene Discovery-Methoden:\n\n");
        messageArea.setCaretPosition(messageArea.getDocument().getLength());
        
        // Methode 1: Lokales Subnetz scannen
        scanLocalSubnet();
        
        // Methode 2: Bekannte Ports testen
        scanCommonPorts();
        
        // Methode 3: Broadcast-Discovery
        sendBroadcast();
        
        // Methode 4: mDNS-ähnliche Discovery
        startMDNSDiscovery();
    }
    
    private void scanLocalSubnet() {
        executor.submit(() -> {
            try {
                String localIP = getLocalIP();
                String subnet = localIP.substring(0, localIP.lastIndexOf('.'));
                
                SwingUtilities.invokeLater(() -> {
                    messageArea.append("📡 Methode 1: Scanne Subnetz " + subnet + ".1-254\n");
                    messageArea.setCaretPosition(messageArea.getDocument().getLength());
                });
                
                for (int i = 1; i <= 254; i++) {
                    final String ip = subnet + "." + i;
                    
                    executor.submit(() -> {
                        if (testEnigmaChatPeer(ip)) {
                            SwingUtilities.invokeLater(() -> {
                                String peerInfo = "🟢 EnigmaChat-Peer (" + ip + ":" + localPort + ")";
                                if (knownPeers.add(peerInfo)) {
                                    peerModel.addElement(peerInfo);
                                    messageArea.append("🎯 EnigmaChat-Peer gefunden: " + ip + "\n");
                                    messageArea.setCaretPosition(messageArea.getDocument().getLength());
                                }
                            });
                        }
                    });
                }
            } catch (Exception e) {
                SwingUtilities.invokeLater(() -> {
                    messageArea.append("❌ Subnetz-Scan fehlgeschlagen: " + e.getMessage() + "\n");
                    messageArea.setCaretPosition(messageArea.getDocument().getLength());
                });
            }
        });
    }
    
    private void scanCommonPorts() {
        executor.submit(() -> {
            SwingUtilities.invokeLater(() -> {
                messageArea.append("📡 Methode 2: Teste EnigmaChat auf Standard-Ports\n");
                messageArea.setCaretPosition(messageArea.getDocument().getLength());
            });
            
            try {
                String localIP = getLocalIP();
                String subnet = localIP.substring(0, localIP.lastIndexOf('.'));
                int[] ports = {8888, 8080, 9999, 7777, 6666};
                
                for (int i = 1; i <= 254; i++) {
                    final String ip = subnet + "." + i;
                    
                    for (int port : ports) {
                        final int testPort = port;
                        executor.submit(() -> {
                            if (testPortConnection(ip, testPort)) {
                                SwingUtilities.invokeLater(() -> {
                                    String peerInfo = "🔵 Peer-Service (" + ip + ":" + testPort + ")";
                                    if (knownPeers.add(peerInfo)) {
                                        peerModel.addElement(peerInfo);
                                        messageArea.append("🎯 Service gefunden: " + ip + ":" + testPort + "\n");
                                        messageArea.setCaretPosition(messageArea.getDocument().getLength());
                                    }
                                });
                            }
                        });
                    }
                }
            } catch (Exception e) {
                SwingUtilities.invokeLater(() -> {
                    messageArea.append("❌ Port-Scan fehlgeschlagen: " + e.getMessage() + "\n");
                    messageArea.setCaretPosition(messageArea.getDocument().getLength());
                });
            }
        });
    }
    
    private void sendBroadcast() {
        executor.submit(() -> {
            try {
                SwingUtilities.invokeLater(() -> {
                    messageArea.append("📡 Methode 3: Sende Broadcast-Discovery\n");
                    messageArea.setCaretPosition(messageArea.getDocument().getLength());
                });
                
                DatagramSocket socket = new DatagramSocket();
                socket.setBroadcast(true);
                
                String message = "ENIGMACHAT_DISCOVERY:" + getLocalIP() + ":" + localPort;
                byte[] buffer = message.getBytes();
                
                InetAddress broadcast = InetAddress.getByName("255.255.255.255");
                DatagramPacket packet = new DatagramPacket(buffer, buffer.length, broadcast, 8889);
                
                socket.send(packet);
                socket.close();
                
                SwingUtilities.invokeLater(() -> {
                    messageArea.append("✅ Broadcast gesendet an alle Netzwerk-Geräte\n");
                    messageArea.setCaretPosition(messageArea.getDocument().getLength());
                });
                
            } catch (Exception e) {
                SwingUtilities.invokeLater(() -> {
                    messageArea.append("❌ Broadcast fehlgeschlagen: " + e.getMessage() + "\n");
                    messageArea.setCaretPosition(messageArea.getDocument().getLength());
                });
            }
        });
    }
    
    private void startMDNSDiscovery() {
        executor.submit(() -> {
            SwingUtilities.invokeLater(() -> {
                messageArea.append("📡 Methode 4: mDNS-ähnliche Discovery\n");
                messageArea.setCaretPosition(messageArea.getDocument().getLength());
            });
            
            try {
                MulticastSocket socket = new MulticastSocket(5353);
                InetAddress group = InetAddress.getByName("224.0.0.251");
                socket.joinGroup(group);
                
                String query = "ENIGMACHAT_QUERY:" + getLocalIP();
                byte[] buffer = query.getBytes();
                DatagramPacket packet = new DatagramPacket(buffer, buffer.length, group, 5353);
                socket.send(packet);
                
                SwingUtilities.invokeLater(() -> {
                    messageArea.append("✅ Multicast-Query gesendet\n");
                    messageArea.setCaretPosition(messageArea.getDocument().getLength());
                });
                
                socket.close();
                
            } catch (Exception e) {
                SwingUtilities.invokeLater(() -> {
                    messageArea.append("⚠️ mDNS nicht verfügbar: " + e.getMessage() + "\n");
                    messageArea.setCaretPosition(messageArea.getDocument().getLength());
                });
            }
        });
    }
    
    private boolean testEnigmaChatPeer(String ip) {
        try (Socket socket = new Socket()) {
            socket.connect(new InetSocketAddress(ip, localPort), 2000);
            
            PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
            out.println("ENIGMACHAT_PING");
            
            return true;
        } catch (Exception e) {
            return false;
        }
    }
    
    private boolean testPortConnection(String ip, int port) {
        try (Socket socket = new Socket()) {
            socket.connect(new InetSocketAddress(ip, port), 1000);
            return true;
        } catch (Exception e) {
            return false;
        }
    }
    
    private String getLocalIP() throws Exception {
        try (DatagramSocket socket = new DatagramSocket()) {
            socket.connect(InetAddress.getByName("8.8.8.8"), 80);
            return socket.getLocalAddress().getHostAddress();
        }
    }
    
    private void sendMessage() {
        String message = messageInput.getText().trim();
        if (message.isEmpty()) return;
        
        if (selectedPeer == null) {
            JOptionPane.showMessageDialog(this, 
                "❌ Bitte wähle zuerst einen Peer aus der Liste!\n\n" +
                "🔍 Starte die Discovery um Peers zu finden.", 
                "Kein Peer ausgewählt", 
                JOptionPane.WARNING_MESSAGE);
            return;
        }
        
        // Extrahiere IP und Port
        String[] parts = extractIPAndPort(selectedPeer);
        String ip = parts[0];
        int port = Integer.parseInt(parts[1]);
        
        messageArea.append("📤 Sende an " + selectedPeer + ":\n");
        messageArea.append("   💬 \"" + message + "\"\n");
        
        executor.submit(() -> {
            try (Socket socket = new Socket()) {
                socket.connect(new InetSocketAddress(ip, port), 5000);
                
                PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
                out.println("EnigmaChat-Message: " + message);
                
                SwingUtilities.invokeLater(() -> {
                    messageArea.append("   ✅ Nachricht erfolgreich gesendet!\n\n");
                    messageArea.setCaretPosition(messageArea.getDocument().getLength());
                });
                
            } catch (Exception e) {
                SwingUtilities.invokeLater(() -> {
                    messageArea.append("   ❌ Verbindung fehlgeschlagen: " + e.getMessage() + "\n\n");
                    messageArea.setCaretPosition(messageArea.getDocument().getLength());
                });
            }
        });
        
        messageInput.setText("");
        messageArea.setCaretPosition(messageArea.getDocument().getLength());
    }
    
    private String[] extractIPAndPort(String peerInfo) {
        int start = peerInfo.indexOf('(') + 1;
        int end = peerInfo.indexOf(')');
        String ipPort = peerInfo.substring(start, end);
        
        if (ipPort.contains(":")) {
            return ipPort.split(":");
        } else {
            return new String[]{ipPort, String.valueOf(localPort)};
        }
    }
    
    private void addManualPeer() {
        String input = JOptionPane.showInputDialog(this, 
            "IP-Adresse eines EnigmaChat-Peers eingeben:\n\n" +
            "Format: 192.168.1.100 oder 192.168.1.100:8888", 
            "Peer manuell hinzufügen", 
            JOptionPane.QUESTION_MESSAGE);
        
        if (input != null && !input.trim().isEmpty()) {
            String[] parts = input.trim().split(":");
            String ip = parts[0];
            int port = parts.length > 1 ? Integer.parseInt(parts[1]) : localPort;
            
            executor.submit(() -> {
                if (testPortConnection(ip, port)) {
                    SwingUtilities.invokeLater(() -> {
                        String peerInfo = "➕ Manueller Peer (" + ip + ":" + port + ")";
                        if (knownPeers.add(peerInfo)) {
                            peerModel.addElement(peerInfo);
                            messageArea.append("✅ Manueller Peer hinzugefügt: " + ip + ":" + port + "\n\n");
                            messageArea.setCaretPosition(messageArea.getDocument().getLength());
                        }
                    });
                } else {
                    SwingUtilities.invokeLater(() -> {
                        messageArea.append("❌ Peer nicht erreichbar: " + ip + ":" + port + "\n\n");
                        messageArea.setCaretPosition(messageArea.getDocument().getLength());
                    });
                }
            });
        }
    }
    
    private void showStatus() {
        try {
            String localIP = getLocalIP();
            String status = 
                "📊 EnigmaChat Status\n\n" +
                "🟢 Server: Läuft auf Port " + localPort + "\n" +
                "🏠 Lokale IP: " + localIP + "\n" +
                "👥 Gefundene Peers: " + knownPeers.size() + "\n" +
                "🔄 Discovery-Methoden: 4 aktiv\n\n" +
                "📡 Port " + localPort + " muss in der Firewall geöffnet sein\n" +
                "🌐 Andere EnigmaChat-Instanzen müssen laufen\n" +
                "🤝 Für beste Ergebnisse: Firewall temporär deaktivieren";
            
            JOptionPane.showMessageDialog(this, status, 
                "📊 Status", JOptionPane.INFORMATION_MESSAGE);
        } catch (Exception e) {
            JOptionPane.showMessageDialog(this, 
                "❌ Fehler beim Status abrufen: " + e.getMessage(), 
                "Fehler", JOptionPane.ERROR_MESSAGE);
        }
    }
    
    private void showInstructions() {
        javax.swing.Timer timer = new javax.swing.Timer(2000, e -> {
            String instructions = 
                "🔍 EnigmaChat Erweiterte Peer-Discovery\n\n" +
                "🎯 SO FUNKTIONIERT ES:\n\n" +
                "1️⃣ Starte EnigmaChat auf mehreren Geräten\n" +
                "2️⃣ Klicke '🔄 Suche starten' für Discovery\n" +
                "3️⃣ Oder '📡 Broadcast senden' für Ankündigung\n" +
                "4️⃣ Füge Peers manuell mit '➕ IP hinzufügen' hinzu\n\n" +
                "🔥 TIPPS:\n" +
                "• Port 8888 in Firewall öffnen\n" +
                "• Auf allen Geräten gleichzeitig starten\n" +
                "• Bei Problemen: Firewall temporär ausschalten\n\n" +
                "🌐 Die App testet 4 verschiedene Discovery-Methoden!";
            
            JOptionPane.showMessageDialog(this, instructions, 
                "🔍 Anleitung", JOptionPane.INFORMATION_MESSAGE);
            ((javax.swing.Timer)e.getSource()).stop();
        });
        timer.setRepeats(false);
        timer.start();
    }
    
    public static void main(String[] args) {
        System.out.println("🔍 Starte EnigmaChat mit erweiterter Peer-Discovery...");
        
        SwingUtilities.invokeLater(() -> {
            try {
                new enigmachat_enhanced_discovery().setVisible(true);
                System.out.println("✅ EnigmaChat bereit für Peer-Discovery!");
            } catch (Exception e) {
                System.err.println("❌ Fehler: " + e.getMessage());
                e.printStackTrace();
            }
        });
    }
}
EOF

echo "✅ EnigmaChat mit verbesserter Discovery erstellt"

# Kompiliere und starte
export JAVA_HOME=/usr/lib/jvm/java-17-openjdk
export PATH=$JAVA_HOME/bin:$PATH

echo "🔧 Kompiliere EnigmaChat Enhanced Discovery..."
javac enigmachat_enhanced_discovery.java

if [ $? -eq 0 ]; then
    echo "✅ Kompilierung erfolgreich!"
    echo ""
    echo "🌐 STARTE ENIGMACHAT MIT VERBESSERTER DISCOVERY"
    echo "=============================================="
    echo "🔍 4 verschiedene Discovery-Methoden aktiv:"
    echo "   1️⃣ Lokales Subnetz scannen"
    echo "   2️⃣ Standard-Ports testen"
    echo "   3️⃣ Broadcast-Discovery"
    echo "   4️⃣ Multicast-Discovery"
    echo ""
    echo "🎯 Server läuft auf Port 8888"
    echo "📡 Öffne Port 8888 in der Firewall für beste Ergebnisse"
    echo ""
    
    java enigmachat_enhanced_discovery
else
    echo "❌ Kompilierungsfehler"
fi