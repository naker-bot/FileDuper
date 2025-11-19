import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.net.*;
import java.io.*;
import java.util.*;
import java.util.List;
import java.util.concurrent.*;
import java.lang.management.ManagementFactory;

public class enigmachat_with_seed extends JFrame {
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
    
    public enigmachat_with_seed() {
        // Hole Prozess-ID
        processId = getProcessId();
        
        knownPeers = new HashSet<>();
        executor = Executors.newFixedThreadPool(20);
        seedRandom = new Random();
        
        initGUI();
        startEnigmaChatServer();
        showWelcomeWithPID();
    }
    
    private long getProcessId() {
        try {
            String jvmName = ManagementFactory.getRuntimeMXBean().getName();
            return Long.parseLong(jvmName.split("@")[0]);
        } catch (Exception e) {
            return System.currentTimeMillis() % 100000; // Fallback
        }
    }
    
    private void initGUI() {
        setTitle("🔑 EnigmaChat - Seed & PID [PID: " + processId + "]");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(1000, 800);
        setLocationRelativeTo(null);
        setLayout(new BorderLayout());
        
        // Top Panel - Seed Input
        JPanel topPanel = new JPanel(new BorderLayout());
        topPanel.setBackground(new Color(33, 37, 41));
        topPanel.setBorder(BorderFactory.createEmptyBorder(10, 10, 5, 10));
        
        JLabel seedLabel = new JLabel("🔑 Seed eingeben:");
        seedLabel.setForeground(Color.WHITE);
        seedLabel.setFont(new Font("Arial", Font.BOLD, 12));
        
        seedInput = new JTextField();
        seedInput.setBackground(new Color(52, 58, 64));
        seedInput.setForeground(Color.WHITE);
        seedInput.setFont(new Font("Monospace", Font.PLAIN, 12));
        seedInput.setBorder(BorderFactory.createEmptyBorder(8, 10, 8, 10));
        seedInput.setToolTipText("Gib einen Seed ein für deterministische Peer-Discovery");
        
        JButton applySeedButton = new JButton("✅ Seed anwenden");
        applySeedButton.setBackground(new Color(40, 167, 69));
        applySeedButton.setForeground(Color.WHITE);
        applySeedButton.setFont(new Font("Arial", Font.BOLD, 11));
        
        JButton randomSeedButton = new JButton("🎲 Zufälliger Seed");
        randomSeedButton.setBackground(new Color(255, 193, 7));
        randomSeedButton.setForeground(Color.BLACK);
        randomSeedButton.setFont(new Font("Arial", Font.BOLD, 11));
        
        JLabel pidLabel = new JLabel("🆔 PID: " + processId);
        pidLabel.setForeground(Color.CYAN);
        pidLabel.setFont(new Font("Monospace", Font.BOLD, 12));
        
        applySeedButton.addActionListener(e -> applySeed());
        randomSeedButton.addActionListener(e -> generateRandomSeed());
        seedInput.addActionListener(e -> applySeed());
        
        JPanel seedPanel = new JPanel(new BorderLayout(5, 0));
        seedPanel.add(seedLabel, BorderLayout.WEST);
        seedPanel.add(seedInput, BorderLayout.CENTER);
        
        JPanel buttonPanel = new JPanel(new FlowLayout(FlowLayout.LEFT, 5, 0));
        buttonPanel.setBackground(new Color(33, 37, 41));
        buttonPanel.add(applySeedButton);
        buttonPanel.add(randomSeedButton);
        buttonPanel.add(pidLabel);
        
        topPanel.add(seedPanel, BorderLayout.CENTER);
        topPanel.add(buttonPanel, BorderLayout.SOUTH);
        add(topPanel, BorderLayout.NORTH);
        
        // Chat-Bereich
        messageArea = new JTextArea();
        messageArea.setEditable(false);
        messageArea.setBackground(new Color(25, 25, 25));
        messageArea.setForeground(Color.GREEN);
        messageArea.setFont(new Font("Monospace", Font.PLAIN, 11));
        messageArea.setText("🔑 EnigmaChat mit Seed-Support gestartet!\n");
        messageArea.append("🆔 Prozess-ID (PID): " + processId + "\n");
        messageArea.append("🌐 Server läuft auf Port " + localPort + "\n");
        messageArea.append("🔑 Gib einen Seed ein für deterministische Discovery\n\n");
        
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
        
        JLabel peerLabel = new JLabel("🌐 Seed-basierte Peers:");
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
        
        inputPanel.add(new JLabel("💬 Nachricht: "), BorderLayout.WEST);
        inputPanel.add(messageInput, BorderLayout.CENTER);
        inputPanel.add(sendButton, BorderLayout.EAST);
        
        // Control Panel
        JPanel controlPanel = new JPanel(new FlowLayout());
        controlPanel.setBackground(new Color(50, 50, 50));
        
        JButton discoverButton = createButton("🔍 Seed-Discovery", new Color(40, 167, 69));
        JButton showInfoButton = createButton("📊 Seed-Info", new Color(108, 117, 125));
        JButton killProcessButton = createButton("💀 Prozess beenden", new Color(220, 53, 69));
        JButton copyPIDButton = createButton("📋 PID kopieren", new Color(23, 162, 184));
        
        discoverButton.addActionListener(e -> startSeedBasedDiscovery());
        showInfoButton.addActionListener(e -> showSeedInfo());
        killProcessButton.addActionListener(e -> killProcess());
        copyPIDButton.addActionListener(e -> copyPIDToClipboard());
        
        controlPanel.add(discoverButton);
        controlPanel.add(showInfoButton);
        controlPanel.add(copyPIDButton);
        controlPanel.add(killProcessButton);
        
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
    
    private void applySeed() {
        String seed = seedInput.getText().trim();
        if (seed.isEmpty()) {
            JOptionPane.showMessageDialog(this, 
                "❌ Bitte gib einen Seed ein!", 
                "Kein Seed", JOptionPane.WARNING_MESSAGE);
            return;
        }
        
        currentSeed = seed;
        seedRandom = new Random(seed.hashCode());
        
        messageArea.append("🔑 Seed angewendet: \"" + seed + "\"\n");
        messageArea.append("🧮 Seed-Hash: " + seed.hashCode() + "\n");
        messageArea.append("🎯 Deterministische Discovery aktiviert\n\n");
        messageArea.setCaretPosition(messageArea.getDocument().getLength());
        
        // Automatisch Discovery starten
        startSeedBasedDiscovery();
    }
    
    private void generateRandomSeed() {
        String[] words = {
            "Alpha", "Beta", "Gamma", "Delta", "Echo", "Foxtrot", 
            "Golf", "Hotel", "India", "Juliet", "Kilo", "Lima",
            "Mike", "November", "Oscar", "Papa", "Quebec", "Romeo",
            "Sierra", "Tango", "Uniform", "Victor", "Whiskey", "X-ray",
            "Yankee", "Zulu", "Crypto", "Enigma", "Cipher", "Quantum"
        };
        
        Random rand = new Random();
        String randomSeed = words[rand.nextInt(words.length)] + 
                           words[rand.nextInt(words.length)] + 
                           (rand.nextInt(9999) + 1000);
        
        seedInput.setText(randomSeed);
        messageArea.append("🎲 Zufälliger Seed generiert: " + randomSeed + "\n\n");
        messageArea.setCaretPosition(messageArea.getDocument().getLength());
    }
    
    private void startSeedBasedDiscovery() {
        if (currentSeed.isEmpty()) {
            JOptionPane.showMessageDialog(this, 
                "❌ Bitte gib zuerst einen Seed ein!", 
                "Kein Seed", JOptionPane.WARNING_MESSAGE);
            return;
        }
        
        peerModel.clear();
        knownPeers.clear();
        
        messageArea.append("🔍 Starte Seed-basierte Discovery...\n");
        messageArea.append("🔑 Verwende Seed: \"" + currentSeed + "\"\n");
        messageArea.append("🎯 Generiere deterministische Peer-IPs\n\n");
        messageArea.setCaretPosition(messageArea.getDocument().getLength());
        
        executor.submit(() -> {
            try {
                String localIP = getLocalIP();
                String subnet = localIP.substring(0, localIP.lastIndexOf('.'));
                
                // Generiere deterministische IPs basierend auf Seed
                Set<Integer> seedIPs = new HashSet<>();
                Random seedRand = new Random(currentSeed.hashCode());
                
                for (int i = 0; i < 20; i++) {
                    int ip = seedRand.nextInt(254) + 1;
                    seedIPs.add(ip);
                }
                
                SwingUtilities.invokeLater(() -> {
                    messageArea.append("🎯 Teste " + seedIPs.size() + " Seed-generierte IPs\n");
                    messageArea.setCaretPosition(messageArea.getDocument().getLength());
                });
                
                for (int ip : seedIPs) {
                    final String targetIP = subnet + "." + ip;
                    
                    executor.submit(() -> {
                        if (testConnection(targetIP)) {
                            SwingUtilities.invokeLater(() -> {
                                String peerInfo = "🔑 Seed-Peer (" + targetIP + ":" + localPort + ")";
                                if (knownPeers.add(peerInfo)) {
                                    peerModel.addElement(peerInfo);
                                    messageArea.append("🎯 Seed-Peer gefunden: " + targetIP + "\n");
                                    messageArea.setCaretPosition(messageArea.getDocument().getLength());
                                }
                            });
                        }
                    });
                }
                
                // Auch reguläre Discovery
                startRegularDiscovery(subnet);
                
            } catch (Exception e) {
                SwingUtilities.invokeLater(() -> {
                    messageArea.append("❌ Discovery-Fehler: " + e.getMessage() + "\n\n");
                    messageArea.setCaretPosition(messageArea.getDocument().getLength());
                });
            }
        });
    }
    
    private void startRegularDiscovery(String subnet) {
        SwingUtilities.invokeLater(() -> {
            messageArea.append("🌐 Zusätzlich: Reguläre Discovery\n");
            messageArea.setCaretPosition(messageArea.getDocument().getLength());
        });
        
        for (int i = 1; i <= 254; i++) {
            final String ip = subnet + "." + i;
            
            executor.submit(() -> {
                if (testConnection(ip)) {
                    SwingUtilities.invokeLater(() -> {
                        String peerInfo = "🌐 Standard-Peer (" + ip + ":" + localPort + ")";
                        if (knownPeers.add(peerInfo)) {
                            peerModel.addElement(peerInfo);
                            messageArea.append("🌐 Standard-Peer: " + ip + "\n");
                            messageArea.setCaretPosition(messageArea.getDocument().getLength());
                        }
                    });
                }
            });
        }
    }
    
    private boolean testConnection(String ip) {
        try (Socket socket = new Socket()) {
            socket.connect(new InetSocketAddress(ip, localPort), 2000);
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
            JOptionPane.showMessageDialog(this, 
                "❌ Bitte wähle zuerst einen Peer aus der Liste!", 
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
                out.println("EnigmaChat[PID:" + processId + "]: " + message);
                
                SwingUtilities.invokeLater(() -> {
                    messageArea.append("   ✅ Gesendet! (von PID " + processId + ")\n\n");
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
    
    private void showSeedInfo() {
        String info = 
            "🔑 Seed-Informationen\n\n" +
            "📝 Aktueller Seed: " + (currentSeed.isEmpty() ? "Nicht gesetzt" : "\"" + currentSeed + "\"") + "\n" +
            "🧮 Seed-Hash: " + (currentSeed.isEmpty() ? "N/A" : String.valueOf(currentSeed.hashCode())) + "\n" +
            "🆔 Prozess-ID: " + processId + "\n" +
            "🌐 Server-Port: " + localPort + "\n" +
            "👥 Gefundene Peers: " + knownPeers.size() + "\n\n" +
            "ℹ️ Der Seed wird verwendet um deterministische\n" +
            "   Peer-IPs zu generieren. Gleiche Seeds finden\n" +
            "   sich gegenseitig leichter!";
        
        JOptionPane.showMessageDialog(this, info, 
            "🔑 Seed-Info", JOptionPane.INFORMATION_MESSAGE);
    }
    
    private void copyPIDToClipboard() {
        try {
            Toolkit.getDefaultToolkit().getSystemClipboard()
                   .setContents(new java.awt.datatransfer.StringSelection(String.valueOf(processId)), null);
            
            messageArea.append("📋 PID " + processId + " in Zwischenablage kopiert\n\n");
            messageArea.setCaretPosition(messageArea.getDocument().getLength());
            
        } catch (Exception e) {
            messageArea.append("❌ Fehler beim Kopieren: " + e.getMessage() + "\n\n");
            messageArea.setCaretPosition(messageArea.getDocument().getLength());
        }
    }
    
    private void killProcess() {
        int choice = JOptionPane.showConfirmDialog(this, 
            "💀 Wirklich den EnigmaChat-Prozess beenden?\n\n" +
            "PID: " + processId + "\n" +
            "Dies wird die Anwendung sofort schließen!",
            "Prozess beenden", 
            JOptionPane.YES_NO_OPTION,
            JOptionPane.WARNING_MESSAGE);
        
        if (choice == JOptionPane.YES_OPTION) {
            messageArea.append("💀 Beende Prozess " + processId + "...\n");
            messageArea.setCaretPosition(messageArea.getDocument().getLength());
            
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {}
            
            System.exit(0);
        }
    }
    
    private void showWelcomeWithPID() {
        javax.swing.Timer timer = new javax.swing.Timer(1500, e -> {
            String welcome = 
                "🔑 EnigmaChat mit Seed-Support\n\n" +
                "🆔 Deine Prozess-ID: " + processId + "\n\n" +
                "🎯 SO FUNKTIONIERT ES:\n\n" +
                "1️⃣ Gib einen Seed ein (z.B. 'MeinSeed123')\n" +
                "2️⃣ Klicke '✅ Seed anwenden'\n" +
                "3️⃣ Starte '🔍 Seed-Discovery'\n" +
                "4️⃣ Peers mit gleichem Seed finden sich!\n\n" +
                "💡 TIPP: Verwende den gleichen Seed auf\n" +
                "   mehreren Geräten für beste Ergebnisse!\n\n" +
                "🆔 Die PID hilft beim Debugging und\n" +
                "   Prozess-Management.";
            
            JOptionPane.showMessageDialog(this, welcome, 
                "🔑 Willkommen - PID: " + processId, JOptionPane.INFORMATION_MESSAGE);
            ((javax.swing.Timer)e.getSource()).stop();
        });
        timer.setRepeats(false);
        timer.start();
    }
    
    public static void main(String[] args) {
        System.out.println("🔑 Starte EnigmaChat mit Seed-Support...");
        
        SwingUtilities.invokeLater(() -> {
            try {
                new enigmachat_with_seed().setVisible(true);
                System.out.println("✅ EnigmaChat bereit! PID verfügbar in der GUI.");
            } catch (Exception e) {
                System.err.println("❌ Fehler: " + e.getMessage());
                e.printStackTrace();
            }
        });
    }
}
