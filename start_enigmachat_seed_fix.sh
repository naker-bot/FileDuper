#!/bin/bash

echo "🔧 ENIGMACHAT - SEED-FIX VERSION"
echo "================================="

cat > /home/nex/c++/enigmachat_seed_fixed.java << 'EOF'
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.net.*;
import java.io.*;
import java.util.*;
import java.util.List;
import java.util.concurrent.*;
import java.lang.management.ManagementFactory;

public class enigmachat_seed_fixed extends JFrame {
    private JTextArea messageArea;
    private JTextField messageInput;
    private JTextField seedInput;
    private JList<String> peerList;
    private DefaultListModel<String> peerModel;
    private String selectedPeer = null;
    private String currentSeed = "";
    private String lastAppliedSeed = "";
    private long processId;
    private ServerSocket serverSocket;
    private boolean isServerRunning = false;
    private ExecutorService executor;
    private Set<String> knownPeers;
    private int localPort = 8888;
    private Random seedRandom;
    private JLabel seedStatusLabel;
    private JButton applySeedButton;
    private int seedApplicationCount = 0;
    
    public enigmachat_seed_fixed() {
        // Hole Prozess-ID
        processId = getProcessId();
        
        knownPeers = new HashSet<>();
        executor = Executors.newFixedThreadPool(20);
        seedRandom = new Random();
        
        initGUI();
        startEnigmaChatServer();
        showWelcomeMessage();
    }
    
    private long getProcessId() {
        try {
            String jvmName = ManagementFactory.getRuntimeMXBean().getName();
            return Long.parseLong(jvmName.split("@")[0]);
        } catch (Exception e) {
            return System.currentTimeMillis() % 100000;
        }
    }
    
    private void initGUI() {
        setTitle("🔧 EnigmaChat - Seed Fix [PID: " + processId + "]");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(1000, 800);
        setLocationRelativeTo(null);
        setLayout(new BorderLayout());
        
        // Top Panel - Seed Input mit Status
        JPanel topPanel = new JPanel(new BorderLayout());
        topPanel.setBackground(new Color(33, 37, 41));
        topPanel.setBorder(BorderFactory.createEmptyBorder(10, 10, 5, 10));
        
        // Seed Input Bereich
        JPanel seedInputPanel = new JPanel(new BorderLayout(5, 0));
        
        JLabel seedLabel = new JLabel("🔑 Seed:");
        seedLabel.setForeground(Color.WHITE);
        seedLabel.setFont(new Font("Arial", Font.BOLD, 12));
        seedLabel.setPreferredSize(new Dimension(60, 30));
        
        seedInput = new JTextField();
        seedInput.setBackground(new Color(52, 58, 64));
        seedInput.setForeground(Color.WHITE);
        seedInput.setFont(new Font("Monospace", Font.PLAIN, 12));
        seedInput.setBorder(BorderFactory.createCompoundBorder(
            BorderFactory.createLineBorder(Color.GRAY),
            BorderFactory.createEmptyBorder(8, 10, 8, 10)
        ));
        seedInput.setToolTipText("Seed eingeben und mehrfach anwenden möglich");
        
        applySeedButton = new JButton("✅ Anwenden");
        applySeedButton.setBackground(new Color(40, 167, 69));
        applySeedButton.setForeground(Color.WHITE);
        applySeedButton.setFont(new Font("Arial", Font.BOLD, 11));
        applySeedButton.setPreferredSize(new Dimension(100, 30));
        
        JButton randomSeedButton = new JButton("🎲 Zufall");
        randomSeedButton.setBackground(new Color(255, 193, 7));
        randomSeedButton.setForeground(Color.BLACK);
        randomSeedButton.setFont(new Font("Arial", Font.BOLD, 11));
        randomSeedButton.setPreferredSize(new Dimension(80, 30));
        
        JButton clearSeedButton = new JButton("🗑️ Clear");
        clearSeedButton.setBackground(new Color(220, 53, 69));
        clearSeedButton.setForeground(Color.WHITE);
        clearSeedButton.setFont(new Font("Arial", Font.BOLD, 11));
        clearSeedButton.setPreferredSize(new Dimension(70, 30));
        
        applySeedButton.addActionListener(e -> applySeedWithForce());
        randomSeedButton.addActionListener(e -> generateAndApplyRandomSeed());
        clearSeedButton.addActionListener(e -> clearSeed());
        seedInput.addActionListener(e -> applySeedWithForce());
        
        JPanel buttonPanel = new JPanel(new FlowLayout(FlowLayout.LEFT, 5, 0));
        buttonPanel.setBackground(new Color(33, 37, 41));
        buttonPanel.add(applySeedButton);
        buttonPanel.add(randomSeedButton);
        buttonPanel.add(clearSeedButton);
        
        seedInputPanel.add(seedLabel, BorderLayout.WEST);
        seedInputPanel.add(seedInput, BorderLayout.CENTER);
        seedInputPanel.add(buttonPanel, BorderLayout.EAST);
        
        // Status Bereich
        JPanel statusPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        statusPanel.setBackground(new Color(33, 37, 41));
        
        seedStatusLabel = new JLabel("❌ Kein Seed aktiv");
        seedStatusLabel.setForeground(Color.RED);
        seedStatusLabel.setFont(new Font("Monospace", Font.BOLD, 11));
        
        JLabel pidLabel = new JLabel("🆔 PID: " + processId);
        pidLabel.setForeground(Color.CYAN);
        pidLabel.setFont(new Font("Monospace", Font.BOLD, 11));
        
        statusPanel.add(seedStatusLabel);
        statusPanel.add(Box.createHorizontalStrut(20));
        statusPanel.add(pidLabel);
        
        topPanel.add(seedInputPanel, BorderLayout.CENTER);
        topPanel.add(statusPanel, BorderLayout.SOUTH);
        add(topPanel, BorderLayout.NORTH);
        
        // Chat-Bereich
        messageArea = new JTextArea();
        messageArea.setEditable(false);
        messageArea.setBackground(new Color(25, 25, 25));
        messageArea.setForeground(Color.GREEN);
        messageArea.setFont(new Font("Monospace", Font.PLAIN, 11));
        messageArea.setText("🔧 EnigmaChat Seed-Fix Version gestartet!\n");
        messageArea.append("🆔 Prozess-ID (PID): " + processId + "\n");
        messageArea.append("🌐 Server läuft auf Port " + localPort + "\n");
        messageArea.append("🔑 Seed kann jetzt mehrfach angewendet werden!\n\n");
        
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
        
        JLabel peerLabel = new JLabel("🌐 Entdeckte Peers:");
        peerLabel.setForeground(Color.WHITE);
        peerLabel.setBorder(BorderFactory.createEmptyBorder(5, 5, 5, 5));
        
        leftPanel.add(peerLabel, BorderLayout.NORTH);
        leftPanel.add(new JScrollPane(peerList), BorderLayout.CENTER);
        leftPanel.setPreferredSize(new Dimension(320, 0));
        add(leftPanel, BorderLayout.WEST);
        
        // Bottom Panel
        JPanel bottomPanel = new JPanel(new BorderLayout());
        
        // Input-Bereich
        JPanel inputPanel = new JPanel(new BorderLayout());
        messageInput = new JTextField();
        messageInput.setBackground(new Color(60, 60, 60));
        messageInput.setForeground(Color.WHITE);
        messageInput.setFont(new Font("Arial", Font.PLAIN, 12));
        messageInput.setBorder(BorderFactory.createEmptyBorder(8, 10, 8, 10));
        
        JButton sendButton = new JButton("📤 Senden");
        sendButton.setBackground(new Color(0, 123, 255));
        sendButton.setForeground(Color.WHITE);
        sendButton.setFont(new Font("Arial", Font.BOLD, 11));
        
        sendButton.addActionListener(e -> sendMessage());
        messageInput.addActionListener(e -> sendMessage());
        
        inputPanel.add(new JLabel("💬 "), BorderLayout.WEST);
        inputPanel.add(messageInput, BorderLayout.CENTER);
        inputPanel.add(sendButton, BorderLayout.EAST);
        
        // Control Panel
        JPanel controlPanel = new JPanel(new FlowLayout());
        controlPanel.setBackground(new Color(50, 50, 50));
        
        JButton discoverButton = createButton("🔍 Discovery starten", new Color(40, 167, 69));
        JButton forceDiscoverButton = createButton("⚡ Force-Discovery", new Color(255, 0, 0));
        JButton showSeedButton = createButton("📊 Seed-Status", new Color(108, 117, 125));
        JButton testSeedButton = createButton("🧪 Seed testen", new Color(156, 39, 176));
        
        discoverButton.addActionListener(e -> startSeedBasedDiscovery());
        forceDiscoverButton.addActionListener(e -> forceDiscovery());
        showSeedButton.addActionListener(e -> showDetailedSeedInfo());
        testSeedButton.addActionListener(e -> testCurrentSeed());
        
        controlPanel.add(discoverButton);
        controlPanel.add(forceDiscoverButton);
        controlPanel.add(showSeedButton);
        controlPanel.add(testSeedButton);
        
        bottomPanel.add(inputPanel, BorderLayout.NORTH);
        bottomPanel.add(controlPanel, BorderLayout.SOUTH);
        add(bottomPanel, BorderLayout.SOUTH);
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
    
    private void applySeedWithForce() {
        String newSeed = seedInput.getText().trim();
        
        if (newSeed.isEmpty()) {
            JOptionPane.showMessageDialog(this, 
                "❌ Bitte gib einen Seed ein!\n\nDer Seed darf nicht leer sein.", 
                "Kein Seed", JOptionPane.WARNING_MESSAGE);
            return;
        }
        
        // Erhöhe Anwendungszähler
        seedApplicationCount++;
        
        // Aktualisiere Seed auch wenn er gleich ist
        String previousSeed = currentSeed;
        currentSeed = newSeed;
        lastAppliedSeed = newSeed;
        
        // Neue Random-Instanz mit Seed + Zeitstempel für Variation
        long seedValue = (newSeed.hashCode() + System.currentTimeMillis()) % Integer.MAX_VALUE;
        seedRandom = new Random(seedValue);
        
        // Update GUI Status
        seedStatusLabel.setText("✅ Seed aktiv: \"" + newSeed + "\" (Anwendung #" + seedApplicationCount + ")");
        seedStatusLabel.setForeground(Color.GREEN);
        applySeedButton.setText("🔄 Neu anwenden");
        
        messageArea.append("🔑 Seed angewendet (Versuch #" + seedApplicationCount + "):\n");
        messageArea.append("   📝 Seed: \"" + newSeed + "\"\n");
        messageArea.append("   🧮 Hash: " + newSeed.hashCode() + "\n");
        messageArea.append("   🎯 Seed-Wert: " + seedValue + "\n");
        
        if (newSeed.equals(previousSeed)) {
            messageArea.append("   🔄 Gleicher Seed erneut angewendet (erlaubt!)\n");
        } else {
            messageArea.append("   🆕 Neuer Seed ersetzt vorherigen\n");
        }
        
        messageArea.append("   ⚡ Bereit für Discovery!\n\n");
        messageArea.setCaretPosition(messageArea.getDocument().getLength());
        
        // Auto-start Discovery nach Seed-Anwendung
        javax.swing.Timer autoDiscovery = new javax.swing.Timer(1000, e -> {
            startSeedBasedDiscovery();
            ((javax.swing.Timer)e.getSource()).stop();
        });
        autoDiscovery.setRepeats(false);
        autoDiscovery.start();
    }
    
    private void generateAndApplyRandomSeed() {
        String[] prefixes = {
            "Alpha", "Beta", "Gamma", "Delta", "Echo", "Foxtrot", 
            "Golf", "Hotel", "India", "Juliet", "Kilo", "Lima",
            "Crypto", "Enigma", "Cipher", "Quantum", "Matrix", "Phoenix"
        };
        
        String[] suffixes = {
            "Chat", "Link", "Net", "Mesh", "Node", "Hub",
            "Core", "Base", "Zone", "Grid", "Web", "Point"
        };
        
        Random rand = new Random();
        String randomSeed = prefixes[rand.nextInt(prefixes.length)] + 
                           suffixes[rand.nextInt(suffixes.length)] + 
                           (rand.nextInt(999) + 100);
        
        seedInput.setText(randomSeed);
        messageArea.append("🎲 Zufälliger Seed generiert: " + randomSeed + "\n");
        messageArea.setCaretPosition(messageArea.getDocument().getLength());
        
        // Automatisch anwenden
        applySeedWithForce();
    }
    
    private void clearSeed() {
        currentSeed = "";
        lastAppliedSeed = "";
        seedApplicationCount = 0;
        seedInput.setText("");
        
        seedStatusLabel.setText("❌ Kein Seed aktiv");
        seedStatusLabel.setForeground(Color.RED);
        applySeedButton.setText("✅ Anwenden");
        
        peerModel.clear();
        knownPeers.clear();
        
        messageArea.append("🗑️ Seed gelöscht und Peer-Liste geleert\n");
        messageArea.append("🔄 Bereit für neuen Seed\n\n");
        messageArea.setCaretPosition(messageArea.getDocument().getLength());
    }
    
    private void startSeedBasedDiscovery() {
        if (currentSeed.isEmpty()) {
            JOptionPane.showMessageDialog(this, 
                "❌ Bitte gib zuerst einen Seed ein!\n\n" +
                "Verwende das Seed-Eingabefeld oben.", 
                "Kein Seed", JOptionPane.WARNING_MESSAGE);
            return;
        }
        
        messageArea.append("🔍 Starte Seed-Discovery (Anwendung #" + seedApplicationCount + ")...\n");
        messageArea.append("🔑 Aktiver Seed: \"" + currentSeed + "\"\n");
        messageArea.append("🎯 Generiere deterministische IPs...\n\n");
        messageArea.setCaretPosition(messageArea.getDocument().getLength());
        
        executor.submit(() -> {
            try {
                String localIP = getLocalIP();
                String subnet = localIP.substring(0, localIP.lastIndexOf('.'));
                
                // Generiere mehr deterministische IPs
                Set<Integer> seedIPs = new HashSet<>();
                Random seedRand = new Random(currentSeed.hashCode() + seedApplicationCount);
                
                // Generiere 30 IPs basierend auf Seed
                for (int i = 0; i < 30; i++) {
                    int ip = seedRand.nextInt(254) + 1;
                    seedIPs.add(ip);
                }
                
                SwingUtilities.invokeLater(() -> {
                    messageArea.append("🎯 Teste " + seedIPs.size() + " Seed-IPs im Subnet " + subnet + ".x\n");
                    messageArea.setCaretPosition(messageArea.getDocument().getLength());
                });
                
                int foundCount = 0;
                for (int ip : seedIPs) {
                    final String targetIP = subnet + "." + ip;
                    
                    if (testConnection(targetIP)) {
                        foundCount++;
                        final int count = foundCount;
                        SwingUtilities.invokeLater(() -> {
                            String peerInfo = "🔑 Seed-Peer #" + count + " (" + targetIP + ":" + localPort + ")";
                            if (knownPeers.add(peerInfo)) {
                                peerModel.addElement(peerInfo);
                                messageArea.append("🎯 Seed-Peer gefunden: " + targetIP + "\n");
                                messageArea.setCaretPosition(messageArea.getDocument().getLength());
                            }
                        });
                    }
                    
                    // Kleine Pause für bessere Performance
                    try { Thread.sleep(50); } catch (InterruptedException e) {}
                }
                
                SwingUtilities.invokeLater(() -> {
                    messageArea.append("✅ Seed-Discovery abgeschlossen! " + foundCount + " Peers gefunden\n\n");
                    messageArea.setCaretPosition(messageArea.getDocument().getLength());
                });
                
            } catch (Exception e) {
                SwingUtilities.invokeLater(() -> {
                    messageArea.append("❌ Discovery-Fehler: " + e.getMessage() + "\n\n");
                    messageArea.setCaretPosition(messageArea.getDocument().getLength());
                });
            }
        });
    }
    
    private void forceDiscovery() {
        messageArea.append("⚡ FORCE-DISCOVERY GESTARTET!\n");
        messageArea.append("🌐 Scanne ALLE IPs im lokalen Netzwerk...\n\n");
        messageArea.setCaretPosition(messageArea.getDocument().getLength());
        
        executor.submit(() -> {
            try {
                String localIP = getLocalIP();
                String subnet = localIP.substring(0, localIP.lastIndexOf('.'));
                
                SwingUtilities.invokeLater(() -> {
                    messageArea.append("⚡ Force-Scan: " + subnet + ".1-254\n");
                    messageArea.setCaretPosition(messageArea.getDocument().getLength());
                });
                
                int foundCount = 0;
                for (int i = 1; i <= 254; i++) {
                    final String ip = subnet + "." + i;
                    
                    if (testConnection(ip)) {
                        foundCount++;
                        final int count = foundCount;
                        SwingUtilities.invokeLater(() -> {
                            String peerInfo = "⚡ Force-Peer #" + count + " (" + ip + ":" + localPort + ")";
                            if (knownPeers.add(peerInfo)) {
                                peerModel.addElement(peerInfo);
                                messageArea.append("⚡ Force-Peer: " + ip + "\n");
                                messageArea.setCaretPosition(messageArea.getDocument().getLength());
                            }
                        });
                    }
                }
                
                SwingUtilities.invokeLater(() -> {
                    messageArea.append("⚡ Force-Discovery fertig! " + foundCount + " Peers total\n\n");
                    messageArea.setCaretPosition(messageArea.getDocument().getLength());
                });
                
            } catch (Exception e) {
                SwingUtilities.invokeLater(() -> {
                    messageArea.append("❌ Force-Discovery Fehler: " + e.getMessage() + "\n\n");
                    messageArea.setCaretPosition(messageArea.getDocument().getLength());
                });
            }
        });
    }
    
    private void testCurrentSeed() {
        if (currentSeed.isEmpty()) {
            JOptionPane.showMessageDialog(this, 
                "❌ Kein Seed zum Testen!\n\nBitte gib zuerst einen Seed ein.", 
                "Kein Seed", JOptionPane.WARNING_MESSAGE);
            return;
        }
        
        messageArea.append("🧪 SEED-TEST GESTARTET\n");
        messageArea.append("🔑 Teste Seed: \"" + currentSeed + "\"\n");
        
        // Generiere Test-IPs
        Random testRand = new Random(currentSeed.hashCode());
        Set<Integer> testIPs = new HashSet<>();
        
        for (int i = 0; i < 10; i++) {
            testIPs.add(testRand.nextInt(254) + 1);
        }
        
        messageArea.append("🎯 Test-IPs: " + testIPs + "\n");
        messageArea.append("🧮 Seed-Hash: " + currentSeed.hashCode() + "\n");
        messageArea.append("📊 Anwendungen: " + seedApplicationCount + "\n\n");
        messageArea.setCaretPosition(messageArea.getDocument().getLength());
    }
    
    private void showDetailedSeedInfo() {
        String info = 
            "🔑 Detaillierte Seed-Informationen\n\n" +
            "📝 Aktueller Seed: " + (currentSeed.isEmpty() ? "Nicht gesetzt" : "\"" + currentSeed + "\"") + "\n" +
            "🔄 Letzter angewendeter Seed: " + (lastAppliedSeed.isEmpty() ? "Keiner" : "\"" + lastAppliedSeed + "\"") + "\n" +
            "📊 Anwendungen: " + seedApplicationCount + "\n" +
            "🧮 Seed-Hash: " + (currentSeed.isEmpty() ? "N/A" : String.valueOf(currentSeed.hashCode())) + "\n" +
            "🆔 Prozess-ID: " + processId + "\n" +
            "🌐 Server-Port: " + localPort + "\n" +
            "👥 Gefundene Peers: " + knownPeers.size() + "\n\n" +
            "✅ SEED-FUNKTIONEN:\n" +
            "• Mehrfache Anwendung möglich\n" +
            "• Automatische Discovery nach Anwendung\n" +
            "• Deterministische IP-Generierung\n" +
            "• Force-Discovery für alle IPs\n\n" +
            "💡 TIPP: Gleiche Seeds auf verschiedenen\n" +
            "   Geräten verwenden für beste Ergebnisse!";
        
        JOptionPane.showMessageDialog(this, info, 
            "🔑 Seed-Details", JOptionPane.INFORMATION_MESSAGE);
    }
    
    private boolean testConnection(String ip) {
        try (Socket socket = new Socket()) {
            socket.connect(new InetSocketAddress(ip, localPort), 1500);
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
    
    private void startEnigmaChatServer() {
        executor.submit(() -> {
            try {
                serverSocket = new ServerSocket(localPort);
                isServerRunning = true;
                
                SwingUtilities.invokeLater(() -> {
                    messageArea.append("🟢 Server gestartet auf Port " + localPort + "\n\n");
                    messageArea.setCaretPosition(messageArea.getDocument().getLength());
                });
                
                while (isServerRunning) {
                    try {
                        Socket clientSocket = serverSocket.accept();
                        handleIncomingConnection(clientSocket);
                    } catch (Exception e) {
                        if (isServerRunning) {
                            SwingUtilities.invokeLater(() -> {
                                messageArea.append("⚠️ Server-Verbindungsfehler: " + e.getMessage() + "\n");
                                messageArea.setCaretPosition(messageArea.getDocument().getLength());
                            });
                        }
                    }
                }
            } catch (Exception e) {
                SwingUtilities.invokeLater(() -> {
                    messageArea.append("❌ Server-Start fehlgeschlagen: " + e.getMessage() + "\n");
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
                    messageArea.setCaretPosition(messageArea.getDocument().getLength());
                });
                
                socket.close();
            } catch (Exception e) {
                // Verbindung geschlossen
            }
        });
    }
    
    private void sendMessage() {
        String message = messageInput.getText().trim();
        if (message.isEmpty()) return;
        
        if (selectedPeer == null) {
            JOptionPane.showMessageDialog(this, 
                "❌ Bitte wähle zuerst einen Peer aus der Liste!\n\n" +
                "Starte Discovery um Peers zu finden.", 
                "Kein Peer ausgewählt", 
                JOptionPane.WARNING_MESSAGE);
            return;
        }
        
        String ip = extractIP(selectedPeer);
        
        messageArea.append("📤 Sende an " + selectedPeer + ":\n");
        messageArea.append("   💬 \"" + message + "\"\n");
        
        executor.submit(() -> {
            try (Socket socket = new Socket()) {
                socket.connect(new InetSocketAddress(ip, localPort), 5000);
                
                PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
                out.println("EnigmaChat[PID:" + processId + "][Seed:" + currentSeed + "]: " + message);
                
                SwingUtilities.invokeLater(() -> {
                    messageArea.append("   ✅ Erfolgreich gesendet!\n\n");
                    messageArea.setCaretPosition(messageArea.getDocument().getLength());
                });
                
            } catch (Exception e) {
                SwingUtilities.invokeLater(() -> {
                    messageArea.append("   ❌ Senden fehlgeschlagen: " + e.getMessage() + "\n\n");
                    messageArea.setCaretPosition(messageArea.getDocument().getLength());
                });
            }
        });
        
        messageInput.setText("");
        messageArea.setCaretPosition(messageArea.getDocument().getLength());
    }
    
    private String extractIP(String peerInfo) {
        int start = peerInfo.indexOf('(') + 1;
        int end = peerInfo.indexOf(':');
        return peerInfo.substring(start, end);
    }
    
    private void showWelcomeMessage() {
        javax.swing.Timer timer = new javax.swing.Timer(2000, e -> {
            String welcome = 
                "🔧 EnigmaChat Seed-Fix Version\n\n" +
                "✅ SEED-PROBLEM BEHOBEN!\n\n" +
                "🎯 NEUE FUNKTIONEN:\n" +
                "• ✅ Seed kann mehrfach angewendet werden\n" +
                "• 🔄 Gleicher Seed erneut anwendbar\n" +
                "• ⚡ Force-Discovery für alle IPs\n" +
                "• 🧪 Seed-Test-Funktionen\n" +
                "• 📊 Detaillierte Seed-Informationen\n\n" +
                "🚀 VERWENDUNG:\n" +
                "1️⃣ Seed eingeben\n" +
                "2️⃣ Mehrfach '✅ Anwenden' klicken (erlaubt!)\n" +
                "3️⃣ Discovery startet automatisch\n" +
                "4️⃣ Bei Problemen: '⚡ Force-Discovery'\n\n" +
                "🆔 Deine PID: " + processId;
            
            JOptionPane.showMessageDialog(this, welcome, 
                "🔧 Seed-Fix - PID: " + processId, JOptionPane.INFORMATION_MESSAGE);
            ((javax.swing.Timer)e.getSource()).stop();
        });
        timer.setRepeats(false);
        timer.start();
    }
    
    public static void main(String[] args) {
        System.out.println("🔧 Starte EnigmaChat Seed-Fix Version...");
        
        SwingUtilities.invokeLater(() -> {
            try {
                new enigmachat_seed_fixed().setVisible(true);
                System.out.println("✅ EnigmaChat Seed-Fix bereit!");
            } catch (Exception e) {
                System.err.println("❌ Fehler: " + e.getMessage());
                e.printStackTrace();
            }
        });
    }
}
EOF

echo "✅ EnigmaChat Seed-Fix Version erstellt"

# Kompiliere und starte
export JAVA_HOME=/usr/lib/jvm/java-17-openjdk
export PATH=$JAVA_HOME/bin:$PATH

echo "🔧 Kompiliere EnigmaChat Seed-Fix..."
javac enigmachat_seed_fixed.java

if [ $? -eq 0 ]; then
    echo "✅ Kompilierung erfolgreich!"
    echo ""
    echo "🔧 STARTE ENIGMACHAT SEED-FIX VERSION"
    echo "====================================="
    echo "✅ Seed kann jetzt mehrfach angewendet werden"
    echo "🔄 Gleicher Seed erneut anwendbar"
    echo "⚡ Force-Discovery für alle IPs verfügbar"
    echo "🧪 Seed-Test-Funktionen integriert"
    echo "📊 Detaillierte Status-Anzeige"
    echo ""
    
    java enigmachat_seed_fixed &
    JAVA_PID=$!
    echo "🚀 EnigmaChat Seed-Fix gestartet mit PID: $JAVA_PID"
    
    wait $JAVA_PID
else
    echo "❌ Kompilierungsfehler"
fi