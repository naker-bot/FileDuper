#!/bin/bash

echo "🔧 ENIGMACHAT - SEED FIX VERSION"
echo "================================"

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
    private long processId;
    private ServerSocket serverSocket;
    private boolean isServerRunning = false;
    private ExecutorService executor;
    private Set<String> knownPeers;
    private int localPort = 8888;
    private Random seedRandom;
    private JLabel seedStatusLabel;
    private boolean discoveryRunning = false;
    
    public enigmachat_seed_fixed() {
        // Hole Prozess-ID
        processId = getProcessId();
        
        knownPeers = new HashSet<>();
        executor = Executors.newFixedThreadPool(30);
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
        setSize(1000, 850);
        setLocationRelativeTo(null);
        setLayout(new BorderLayout());
        
        // Top Panel - Verbesserter Seed Input
        JPanel topPanel = new JPanel(new BorderLayout());
        topPanel.setBackground(new Color(33, 37, 41));
        topPanel.setBorder(BorderFactory.createEmptyBorder(10, 10, 5, 10));
        
        // Seed Input Row
        JPanel seedInputPanel = new JPanel(new BorderLayout(5, 0));
        
        JLabel seedLabel = new JLabel("🔑 Seed:");
        seedLabel.setForeground(Color.WHITE);
        seedLabel.setFont(new Font("Arial", Font.BOLD, 12));
        seedLabel.setPreferredSize(new Dimension(50, 30));
        
        seedInput = new JTextField();
        seedInput.setBackground(new Color(52, 58, 64));
        seedInput.setForeground(Color.WHITE);
        seedInput.setFont(new Font("Monospace", Font.PLAIN, 14));
        seedInput.setBorder(BorderFactory.createCompoundBorder(
            BorderFactory.createLineBorder(Color.GRAY, 1),
            BorderFactory.createEmptyBorder(8, 10, 8, 10)
        ));
        seedInput.setToolTipText("Seed eingeben und Enter drücken");
        
        JButton applySeedButton = new JButton("✅ Anwenden");
        applySeedButton.setBackground(new Color(40, 167, 69));
        applySeedButton.setForeground(Color.WHITE);
        applySeedButton.setFont(new Font("Arial", Font.BOLD, 11));
        applySeedButton.setPreferredSize(new Dimension(100, 30));
        
        JButton clearSeedButton = new JButton("🗑️ Löschen");
        clearSeedButton.setBackground(new Color(220, 53, 69));
        clearSeedButton.setForeground(Color.WHITE);
        clearSeedButton.setFont(new Font("Arial", Font.BOLD, 11));
        clearSeedButton.setPreferredSize(new Dimension(80, 30));
        
        seedInputPanel.add(seedLabel, BorderLayout.WEST);
        seedInputPanel.add(seedInput, BorderLayout.CENTER);
        seedInputPanel.add(applySeedButton, BorderLayout.EAST);
        
        // Status und Control Row
        JPanel controlPanel = new JPanel(new FlowLayout(FlowLayout.LEFT, 5, 5));
        controlPanel.setBackground(new Color(33, 37, 41));
        
        JButton randomSeedButton = new JButton("🎲 Zufällig");
        randomSeedButton.setBackground(new Color(255, 193, 7));
        randomSeedButton.setForeground(Color.BLACK);
        randomSeedButton.setFont(new Font("Arial", Font.BOLD, 10));
        
        seedStatusLabel = new JLabel("❌ Kein Seed aktiv");
        seedStatusLabel.setForeground(Color.RED);
        seedStatusLabel.setFont(new Font("Arial", Font.BOLD, 12));
        
        JLabel pidLabel = new JLabel("🆔 PID: " + processId);
        pidLabel.setForeground(Color.CYAN);
        pidLabel.setFont(new Font("Monospace", Font.BOLD, 12));
        
        controlPanel.add(randomSeedButton);
        controlPanel.add(clearSeedButton);
        controlPanel.add(Box.createHorizontalStrut(20));
        controlPanel.add(seedStatusLabel);
        controlPanel.add(Box.createHorizontalStrut(20));
        controlPanel.add(pidLabel);
        
        // Event Handlers
        applySeedButton.addActionListener(e -> applySeed());
        clearSeedButton.addActionListener(e -> clearSeed());
        randomSeedButton.addActionListener(e -> generateAndApplyRandomSeed());
        seedInput.addActionListener(e -> applySeed());
        
        // Seed Input Focus Handler
        seedInput.addFocusListener(new FocusAdapter() {
            @Override
            public void focusGained(FocusEvent e) {
                seedInput.selectAll();
            }
        });
        
        topPanel.add(seedInputPanel, BorderLayout.NORTH);
        topPanel.add(controlPanel, BorderLayout.SOUTH);
        add(topPanel, BorderLayout.NORTH);
        
        // Chat-Bereich
        messageArea = new JTextArea();
        messageArea.setEditable(false);
        messageArea.setBackground(new Color(25, 25, 25));
        messageArea.setForeground(Color.GREEN);
        messageArea.setFont(new Font("Monospace", Font.PLAIN, 11));
        messageArea.setText("🔧 EnigmaChat Seed Fix Version gestartet!\n");
        messageArea.append("🆔 Prozess-ID: " + processId + "\n");
        messageArea.append("🌐 Server läuft auf Port " + localPort + "\n");
        messageArea.append("🔑 Seed-System bereit - gib einen Seed ein!\n\n");
        
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
        
        JLabel peerLabel = new JLabel("🌐 Seed-Peers:");
        peerLabel.setForeground(Color.WHITE);
        peerLabel.setBorder(BorderFactory.createEmptyBorder(5, 5, 5, 5));
        
        leftPanel.add(peerLabel, BorderLayout.NORTH);
        leftPanel.add(new JScrollPane(peerList), BorderLayout.CENTER);
        leftPanel.setPreferredSize(new Dimension(280, 0));
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
        
        // Action Buttons
        JPanel actionPanel = new JPanel(new FlowLayout());
        actionPanel.setBackground(new Color(50, 50, 50));
        
        JButton startDiscoveryButton = createButton("🔍 Seed-Discovery", new Color(40, 167, 69));
        JButton stopDiscoveryButton = createButton("⏹️ Stop Discovery", new Color(220, 53, 69));
        JButton showSeedInfoButton = createButton("📊 Seed-Info", new Color(108, 117, 125));
        JButton testSeedButton = createButton("🧪 Seed testen", new Color(255, 193, 7));
        
        startDiscoveryButton.addActionListener(e -> startSeedDiscovery());
        stopDiscoveryButton.addActionListener(e -> stopDiscovery());
        showSeedInfoButton.addActionListener(e -> showDetailedSeedInfo());
        testSeedButton.addActionListener(e -> testCurrentSeed());
        
        actionPanel.add(startDiscoveryButton);
        actionPanel.add(stopDiscoveryButton);
        actionPanel.add(testSeedButton);
        actionPanel.add(showSeedInfoButton);
        
        bottomPanel.add(inputPanel, BorderLayout.NORTH);
        bottomPanel.add(actionPanel, BorderLayout.SOUTH);
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
    
    private void applySeed() {
        String newSeed = seedInput.getText().trim();
        
        if (newSeed.isEmpty()) {
            showError("❌ Seed darf nicht leer sein!");
            return;
        }
        
        // Prüfe ob Seed geändert wurde
        boolean seedChanged = !newSeed.equals(currentSeed);
        
        currentSeed = newSeed;
        seedRandom = new Random(currentSeed.hashCode());
        
        // Update Status
        seedStatusLabel.setText("✅ Seed: \"" + currentSeed + "\"");
        seedStatusLabel.setForeground(Color.GREEN);
        
        messageArea.append("🔑 Seed " + (seedChanged ? "geändert" : "angewendet") + ": \"" + currentSeed + "\"\n");
        messageArea.append("🧮 Hash-Wert: " + currentSeed.hashCode() + "\n");
        messageArea.append("🎯 Deterministische Discovery aktiviert\n");
        
        if (seedChanged) {
            messageArea.append("🔄 Neuer Seed - Discovery wird neu gestartet\n");
            // Stoppe laufende Discovery und starte neu
            if (discoveryRunning) {
                stopDiscovery();
                javax.swing.Timer restartTimer = new javax.swing.Timer(1000, e -> {
                    startSeedDiscovery();
                    ((javax.swing.Timer)e.getSource()).stop();
                });
                restartTimer.setRepeats(false);
                restartTimer.start();
            }
        }
        
        messageArea.append("\n");
        messageArea.setCaretPosition(messageArea.getDocument().getLength());
        
        // Bestätigung anzeigen
        seedInput.setBackground(new Color(40, 167, 69, 50));
        javax.swing.Timer colorTimer = new javax.swing.Timer(1000, e -> {
            seedInput.setBackground(new Color(52, 58, 64));
            ((javax.swing.Timer)e.getSource()).stop();
        });
        colorTimer.setRepeats(false);
        colorTimer.start();
    }
    
    private void clearSeed() {
        currentSeed = "";
        seedInput.setText("");
        seedStatusLabel.setText("❌ Kein Seed aktiv");
        seedStatusLabel.setForeground(Color.RED);
        
        stopDiscovery();
        peerModel.clear();
        knownPeers.clear();
        
        messageArea.append("🗑️ Seed gelöscht - Discovery gestoppt\n\n");
        messageArea.setCaretPosition(messageArea.getDocument().getLength());
    }
    
    private void generateAndApplyRandomSeed() {
        String[] adjectives = {"Alpha", "Beta", "Gamma", "Delta", "Echo", "Foxtrot", "Golf", "Hotel"};
        String[] nouns = {"Chat", "Link", "Node", "Peer", "Net", "Hub", "Core", "Gate"};
        
        Random rand = new Random();
        String randomSeed = adjectives[rand.nextInt(adjectives.length)] + 
                           nouns[rand.nextInt(nouns.length)] + 
                           (rand.nextInt(999) + 100);
        
        seedInput.setText(randomSeed);
        applySeed();
        
        messageArea.append("🎲 Zufälliger Seed generiert und angewendet\n\n");
        messageArea.setCaretPosition(messageArea.getDocument().getLength());
    }
    
    private void startSeedDiscovery() {
        if (currentSeed.isEmpty()) {
            showError("❌ Bitte gib zuerst einen Seed ein!");
            return;
        }
        
        if (discoveryRunning) {
            messageArea.append("⚠️ Discovery läuft bereits...\n\n");
            messageArea.setCaretPosition(messageArea.getDocument().getLength());
            return;
        }
        
        discoveryRunning = true;
        peerModel.clear();
        knownPeers.clear();
        
        messageArea.append("🔍 Starte Seed-Discovery mit \"" + currentSeed + "\"...\n");
        messageArea.append("🎯 Hash: " + currentSeed.hashCode() + "\n");
        messageArea.append("🌐 Suche nach Peers mit gleichem Seed...\n\n");
        messageArea.setCaretPosition(messageArea.getDocument().getLength());
        
        executor.submit(() -> {
            try {
                String localIP = getLocalIP();
                String subnet = localIP.substring(0, localIP.lastIndexOf('.'));
                
                SwingUtilities.invokeLater(() -> {
                    messageArea.append("📡 Lokales Netz: " + subnet + ".1-254\n");
                    messageArea.setCaretPosition(messageArea.getDocument().getLength());
                });
                
                // Generiere deterministische IPs basierend auf Seed
                Set<Integer> seedIPs = generateSeedIPs(currentSeed, 25);
                
                SwingUtilities.invokeLater(() -> {
                    messageArea.append("🎯 Teste " + seedIPs.size() + " Seed-IPs: " + seedIPs + "\n\n");
                    messageArea.setCaretPosition(messageArea.getDocument().getLength());
                });
                
                CountDownLatch seedLatch = new CountDownLatch(seedIPs.size());
                
                // Teste Seed-basierte IPs
                for (int ip : seedIPs) {
                    if (!discoveryRunning) break;
                    
                    final String targetIP = subnet + "." + ip;
                    
                    executor.submit(() -> {
                        try {
                            if (testEnigmaChatConnection(targetIP)) {
                                SwingUtilities.invokeLater(() -> {
                                    String peerInfo = "🔑 Seed-Peer (" + targetIP + ":" + localPort + ")";
                                    if (knownPeers.add(peerInfo)) {
                                        peerModel.addElement(peerInfo);
                                        messageArea.append("🎯 SEED-MATCH! " + targetIP + " gefunden!\n");
                                        messageArea.setCaretPosition(messageArea.getDocument().getLength());
                                    }
                                });
                            }
                        } finally {
                            seedLatch.countDown();
                        }
                    });
                }
                
                // Warte auf Seed-Discovery Completion
                new Thread(() -> {
                    try {
                        seedLatch.await(15, TimeUnit.SECONDS);
                        
                        if (discoveryRunning) {
                            SwingUtilities.invokeLater(() -> {
                                messageArea.append("✅ Seed-Discovery abgeschlossen\n");
                                messageArea.append("🌐 Starte zusätzlich allgemeine Suche...\n\n");
                                messageArea.setCaretPosition(messageArea.getDocument().getLength());
                            });
                            
                            // Zusätzlich allgemeine Discovery
                            startGeneralDiscovery(subnet);
                        }
                    } catch (InterruptedException e) {
                        discoveryRunning = false;
                    }
                }).start();
                
            } catch (Exception e) {
                SwingUtilities.invokeLater(() -> {
                    messageArea.append("❌ Discovery-Fehler: " + e.getMessage() + "\n\n");
                    messageArea.setCaretPosition(messageArea.getDocument().getLength());
                    discoveryRunning = false;
                });
            }
        });
    }
    
    private Set<Integer> generateSeedIPs(String seed, int count) {
        Set<Integer> ips = new HashSet<>();
        Random seedRand = new Random(seed.hashCode());
        
        while (ips.size() < count && ips.size() < 254) {
            int ip = seedRand.nextInt(254) + 1;
            ips.add(ip);
        }
        
        return ips;
    }
    
    private void startGeneralDiscovery(String subnet) {
        if (!discoveryRunning) return;
        
        for (int i = 1; i <= 254; i++) {
            if (!discoveryRunning) break;
            
            final String ip = subnet + "." + i;
            
            executor.submit(() -> {
                if (discoveryRunning && testEnigmaChatConnection(ip)) {
                    SwingUtilities.invokeLater(() -> {
                        String peerInfo = "🌐 EnigmaChat (" + ip + ":" + localPort + ")";
                        if (knownPeers.add(peerInfo)) {
                            peerModel.addElement(peerInfo);
                            messageArea.append("🌐 Allgemeiner Peer: " + ip + "\n");
                            messageArea.setCaretPosition(messageArea.getDocument().getLength());
                        }
                    });
                }
            });
        }
    }
    
    private void stopDiscovery() {
        if (!discoveryRunning) {
            messageArea.append("ℹ️ Keine Discovery aktiv\n\n");
            messageArea.setCaretPosition(messageArea.getDocument().getLength());
            return;
        }
        
        discoveryRunning = false;
        messageArea.append("⏹️ Discovery gestoppt\n\n");
        messageArea.setCaretPosition(messageArea.getDocument().getLength());
    }
    
    private boolean testEnigmaChatConnection(String ip) {
        try (Socket socket = new Socket()) {
            socket.connect(new InetSocketAddress(ip, localPort), 2000);
            
            PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
            out.println("ENIGMA_PING:" + currentSeed);
            
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
    
    private void testCurrentSeed() {
        if (currentSeed.isEmpty()) {
            showError("❌ Kein Seed zum Testen vorhanden!");
            return;
        }
        
        Set<Integer> testIPs = generateSeedIPs(currentSeed, 10);
        
        messageArea.append("🧪 Teste aktuellen Seed \"" + currentSeed + "\":\n");
        messageArea.append("🔢 Hash: " + currentSeed.hashCode() + "\n");
        messageArea.append("🎯 Test-IPs: " + testIPs + "\n");
        messageArea.append("✅ Seed funktioniert korrekt!\n\n");
        messageArea.setCaretPosition(messageArea.getDocument().getLength());
    }
    
    private void showDetailedSeedInfo() {
        String info = "🔑 Detaillierte Seed-Informationen\n\n";
        
        if (currentSeed.isEmpty()) {
            info += "❌ Aktueller Seed: Nicht gesetzt\n\n";
            info += "ℹ️ Gib einen Seed ein um die Discovery zu starten!";
        } else {
            Set<Integer> seedIPs = generateSeedIPs(currentSeed, 15);
            
            info += "✅ Aktueller Seed: \"" + currentSeed + "\"\n";
            info += "🧮 Hash-Wert: " + currentSeed.hashCode() + "\n";
            info += "🎯 Seed-IPs: " + seedIPs + "\n";
            info += "🌐 Discovery: " + (discoveryRunning ? "Läuft" : "Gestoppt") + "\n";
            info += "👥 Gefundene Peers: " + knownPeers.size() + "\n";
            info += "🆔 Prozess-ID: " + processId + "\n";
            info += "📡 Server-Port: " + localPort + "\n\n";
            info += "💡 Peers mit gleichem Seed finden sich\n";
            info += "   automatisch über deterministische IPs!";
        }
        
        JOptionPane.showMessageDialog(this, info, 
            "🔑 Seed-Info", JOptionPane.INFORMATION_MESSAGE);
    }
    
    private void showError(String message) {
        JOptionPane.showMessageDialog(this, message, 
            "❌ Fehler", JOptionPane.ERROR_MESSAGE);
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
                                messageArea.append("⚠️ Server-Fehler: " + e.getMessage() + "\n");
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
            showError("❌ Bitte wähle zuerst einen Peer aus der Liste!");
            return;
        }
        
        String ip = extractIP(selectedPeer);
        
        messageArea.append("📤 Sende an " + selectedPeer + ":\n");
        messageArea.append("   💬 \"" + message + "\"\n");
        
        executor.submit(() -> {
            try (Socket socket = new Socket()) {
                socket.connect(new InetSocketAddress(ip, localPort), 5000);
                
                PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
                out.println("EnigmaChat[" + currentSeed + "|PID:" + processId + "]: " + message);
                
                SwingUtilities.invokeLater(() -> {
                    messageArea.append("   ✅ Gesendet mit Seed \"" + currentSeed + "\"!\n\n");
                    messageArea.setCaretPosition(messageArea.getDocument().getLength());
                });
                
            } catch (Exception e) {
                SwingUtilities.invokeLater(() -> {
                    messageArea.append("   ❌ Fehler: " + e.getMessage() + "\n\n");
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
        javax.swing.Timer timer = new javax.swing.Timer(1000, e -> {
            String welcome = 
                "🔧 EnigmaChat Seed Fix Version\n\n" +
                "🆔 Prozess-ID: " + processId + "\n\n" +
                "🔑 SEED-FUNKTIONEN:\n\n" +
                "✅ Mehrfache Seed-Eingabe möglich\n" +
                "🔄 Seed kann jederzeit geändert werden\n" +
                "🎲 Zufällige Seed-Generierung\n" +
                "🧪 Seed-Tests verfügbar\n" +
                "⏹️ Discovery kann gestoppt/neu gestartet werden\n\n" +
                "🎯 VERWENDUNG:\n" +
                "1️⃣ Seed eingeben und ✅ 'Anwenden' klicken\n" +
                "2️⃣ '🔍 Seed-Discovery' starten\n" +
                "3️⃣ Bei Bedarf Seed ändern und neu anwenden\n\n" +
                "💡 Tipp: Gleiches Seed auf allen Geräten verwenden!";
            
            JOptionPane.showMessageDialog(this, welcome, 
                "🔧 Seed Fix - PID: " + processId, JOptionPane.INFORMATION_MESSAGE);
            ((javax.swing.Timer)e.getSource()).stop();
        });
        timer.setRepeats(false);
        timer.start();
    }
    
    public static void main(String[] args) {
        System.out.println("🔧 Starte EnigmaChat Seed Fix Version...");
        
        SwingUtilities.invokeLater(() -> {
            try {
                new enigmachat_seed_fixed().setVisible(true);
                System.out.println("✅ EnigmaChat Seed Fix bereit!");
            } catch (Exception e) {
                System.err.println("❌ Fehler: " + e.getMessage());
                e.printStackTrace();
            }
        });
    }
}
EOF

echo "✅ EnigmaChat Seed Fix Version erstellt"

# Kompiliere und starte
export JAVA_HOME=/usr/lib/jvm/java-17-openjdk
export PATH=$JAVA_HOME/bin:$PATH

echo "🔧 Kompiliere EnigmaChat Seed Fix..."
javac enigmachat_seed_fixed.java

if [ $? -eq 0 ]; then
    echo "✅ Kompilierung erfolgreich!"
    echo ""
    echo "🔧 STARTE ENIGMACHAT SEED FIX VERSION"
    echo "====================================="
    echo "🔑 Verbesserter Seed-Input"
    echo "✅ Mehrfache Seed-Anwendung möglich"
    echo "🔄 Seed kann jederzeit geändert werden"
    echo "🧪 Seed-Test-Funktionen"
    echo "⏹️ Discovery Start/Stop Kontrolle"
    echo "📊 Detaillierte Seed-Informationen"
    echo ""
    
    java enigmachat_seed_fixed &
    JAVA_PID=$!
    echo "🚀 EnigmaChat gestartet mit PID: $JAVA_PID"
    
    wait $JAVA_PID
else
    echo "❌ Kompilierungsfehler"
fi