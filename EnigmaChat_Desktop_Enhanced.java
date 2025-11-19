import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.security.MessageDigest;
import java.net.*;
import java.util.*;
import java.util.List;
import java.util.concurrent.*;
import java.util.concurrent.atomic.AtomicInteger;

public class EnigmaChat_Desktop_Enhanced extends JFrame {
    
    private JTextField seedInput;
    private JButton applyButton, testButton, clearButton, generateButton;
    private JTextArea statusArea, resultsArea;
    private JProgressBar progressBar;
    private JLabel statusLabel;
    
    private String currentSeed = "";
    private List<String> generatedIPs = new ArrayList<>();
    private List<String> activeConnections = new ArrayList<>();
    private ExecutorService executor = Executors.newFixedThreadPool(10);
    private AtomicInteger testProgress = new AtomicInteger(0);
    private boolean isRangeTesting = false;
    
    public EnigmaChat_Desktop_Enhanced() {
        setTitle("EnigmaChat Enhanced Seed - Desktop Version");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(800, 700);
        setLocationRelativeTo(null);
        
        createUI();
        setupListeners();
        updateStatus("✅ EnigmaChat Enhanced bereit - Seed eingeben");
    }
    
    private void createUI() {
        setLayout(new BorderLayout());
        
        // Header
        JPanel headerPanel = new JPanel(new FlowLayout());
        headerPanel.setBackground(new Color(33, 150, 243));
        JLabel titleLabel = new JLabel("🎲 EnigmaChat Enhanced Seed");
        titleLabel.setFont(new Font("Arial", Font.BOLD, 20));
        titleLabel.setForeground(Color.WHITE);
        headerPanel.add(titleLabel);
        add(headerPanel, BorderLayout.NORTH);
        
        // Main Panel
        JPanel mainPanel = new JPanel(new GridBagLayout());
        GridBagConstraints gbc = new GridBagConstraints();
        mainPanel.setBorder(BorderFactory.createEmptyBorder(20, 20, 20, 20));
        
        // Seed Input Section
        gbc.gridx = 0; gbc.gridy = 0; gbc.gridwidth = 4; gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.insets = new Insets(5, 5, 5, 5);
        
        JPanel seedPanel = new JPanel(new BorderLayout());
        seedPanel.setBorder(BorderFactory.createTitledBorder("🎲 Seed-Eingabe"));
        
        seedInput = new JTextField("mein-geheimer-schluessel");
        seedInput.setFont(new Font("Monospace", Font.PLAIN, 14));
        seedPanel.add(seedInput, BorderLayout.CENTER);
        
        JPanel buttonPanel = new JPanel(new FlowLayout());
        applyButton = new JButton("✅ Anwenden");
        clearButton = new JButton("🧹 Löschen");
        generateButton = new JButton("🎲 Zufällig");
        
        applyButton.setFont(new Font("Arial", Font.BOLD, 12));
        clearButton.setFont(new Font("Arial", Font.BOLD, 12));
        generateButton.setFont(new Font("Arial", Font.BOLD, 12));
        
        buttonPanel.add(applyButton);
        buttonPanel.add(clearButton);
        buttonPanel.add(generateButton);
        seedPanel.add(buttonPanel, BorderLayout.SOUTH);
        
        mainPanel.add(seedPanel, gbc);
        
        // Test Section
        gbc.gridy = 1;
        JPanel testPanel = new JPanel(new BorderLayout());
        testPanel.setBorder(BorderFactory.createTitledBorder("🔍 IP-Range Testing"));
        
        testButton = new JButton("🔍 Test (Seed erforderlich)");
        testButton.setFont(new Font("Arial", Font.BOLD, 14));
        testButton.setEnabled(false);
        testPanel.add(testButton, BorderLayout.CENTER);
        
        JLabel hintLabel = new JLabel("💡 Rechtsklick für vollständigen Range-Test");
        hintLabel.setFont(new Font("Arial", Font.ITALIC, 10));
        testPanel.add(hintLabel, BorderLayout.SOUTH);
        
        mainPanel.add(testPanel, gbc);
        
        // Progress Section
        gbc.gridy = 2;
        progressBar = new JProgressBar();
        progressBar.setStringPainted(true);
        progressBar.setVisible(false);
        mainPanel.add(progressBar, gbc);
        
        // Status Section
        gbc.gridy = 3;
        statusLabel = new JLabel("📱 Status: Bereit");
        statusLabel.setFont(new Font("Arial", Font.BOLD, 14));
        statusLabel.setBorder(BorderFactory.createEtchedBorder());
        statusLabel.setOpaque(true);
        statusLabel.setBackground(new Color(227, 242, 253));
        mainPanel.add(statusLabel, gbc);
        
        // Results Section
        gbc.gridy = 4; gbc.weighty = 1.0; gbc.fill = GridBagConstraints.BOTH;
        
        JTabbedPane tabbedPane = new JTabbedPane();
        
        resultsArea = new JTextArea();
        resultsArea.setFont(new Font("Monospace", Font.PLAIN, 12));
        resultsArea.setEditable(false);
        resultsArea.setText("🎯 Hier erscheinen die generierten IPs und Test-Ergebnisse...");
        JScrollPane resultsScroll = new JScrollPane(resultsArea);
        tabbedPane.addTab("📊 Ergebnisse", resultsScroll);
        
        statusArea = new JTextArea();
        statusArea.setFont(new Font("Monospace", Font.PLAIN, 11));
        statusArea.setEditable(false);
        statusArea.setText("📋 Detaillierte Logs erscheinen hier...\n");
        JScrollPane statusScroll = new JScrollPane(statusArea);
        tabbedPane.addTab("📋 Logs", statusScroll);
        
        mainPanel.add(tabbedPane, gbc);
        
        add(mainPanel, BorderLayout.CENTER);
    }
    
    private void setupListeners() {
        applyButton.addActionListener(e -> applySeed());
        testButton.addActionListener(e -> testSingleIP());
        clearButton.addActionListener(e -> clearSeed());
        generateButton.addActionListener(e -> generateRandomSeed());
        
        // Right-click for range test
        testButton.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                if (SwingUtilities.isRightMouseButton(e)) {
                    testAllSeedIPs();
                }
            }
        });
        
        seedInput.addActionListener(e -> applySeed());
    }
    
    private void applySeed() {
        String seed = seedInput.getText().trim();
        if (seed.isEmpty()) {
            updateStatus("❌ Kein Seed eingegeben");
            return;
        }
        
        currentSeed = seed;
        generatedIPs = generateSeedBasedIPs(seed);
        
        updateStatus("✅ Seed '" + seed + "' angewendet - " + generatedIPs.size() + " IPs generiert");
        displayGeneratedIPs();
        
        testButton.setEnabled(true);
        testButton.setText("🔍 Test ersten IP");
        
        logMessage("🎲 Seed angewendet: " + seed);
        logMessage("📝 " + generatedIPs.size() + " IPs generiert");
    }
    
    private List<String> generateSeedBasedIPs(String seed) {
        List<String> ips = new ArrayList<>();
        
        try {
            MessageDigest digest = MessageDigest.getInstance("SHA-256");
            byte[] hash = digest.digest(seed.getBytes());
            
            // Generate 25 deterministic IPs
            for (int i = 0; i < 25; i++) {
                int byte3 = (hash[i % hash.length] & 0xFF) % 255 + 1;
                int byte4 = (hash[(i + 1) % hash.length] & 0xFF) % 254 + 1;
                ips.add("192.168." + byte3 + "." + byte4);
            }
            
            // Add local network IPs
            for (int i = 1; i <= 10; i++) {
                ips.add("192.168.1." + i);
            }
            
        } catch (Exception e) {
            // Fallback
            for (int i = 1; i <= 25; i++) {
                ips.add("192.168.1." + i);
            }
        }
        
        return new ArrayList<>(new LinkedHashSet<>(ips)); // Remove duplicates
    }
    
    private void displayGeneratedIPs() {
        StringBuilder sb = new StringBuilder();
        sb.append("🎲 Generierte IPs für Seed '").append(currentSeed).append("':\n\n");
        
        for (int i = 0; i < generatedIPs.size(); i++) {
            sb.append(String.format("%2d. %s\n", i + 1, generatedIPs.get(i)));
        }
        
        sb.append("\n💡 Rechtsklick 'Test' für vollständigen Range-Scan");
        resultsArea.setText(sb.toString());
    }
    
    private void testSingleIP() {
        if (generatedIPs.isEmpty()) {
            updateStatus("❌ Erst Seed anwenden");
            return;
        }
        
        String testIP = generatedIPs.get(0);
        updateStatus("🔍 Teste " + testIP + "...");
        progressBar.setVisible(true);
        progressBar.setIndeterminate(true);
        
        executor.execute(() -> {
            boolean isReachable = testConnection(testIP);
            SwingUtilities.invokeLater(() -> {
                progressBar.setVisible(false);
                String result = isReachable ? "✅ " + testIP + " ist erreichbar!" : "❌ " + testIP + " nicht erreichbar";
                updateStatus(result);
                logMessage(result);
            });
        });
    }
    
    private void testAllSeedIPs() {
        if (generatedIPs.isEmpty()) {
            updateStatus("❌ Erst Seed anwenden");
            return;
        }
        
        if (isRangeTesting) {
            updateStatus("⚠️ Range-Test läuft bereits");
            return;
        }
        
        isRangeTesting = true;
        testProgress.set(0);
        activeConnections.clear();
        
        updateStatus("🔍 Starte vollständigen Range-Test für " + generatedIPs.size() + " IPs...");
        progressBar.setVisible(true);
        progressBar.setMaximum(generatedIPs.size());
        progressBar.setValue(0);
        progressBar.setIndeterminate(false);
        
        testButton.setText("⏳ Testing...");
        testButton.setEnabled(false);
        
        long startTime = System.currentTimeMillis();
        logMessage("🚀 Range-Test gestartet - " + generatedIPs.size() + " IPs zu testen");
        
        for (String ip : generatedIPs) {
            executor.execute(() -> {
                boolean isReachable = testConnection(ip);
                
                SwingUtilities.invokeLater(() -> {
                    int progress = testProgress.incrementAndGet();
                    progressBar.setValue(progress);
                    
                    if (isReachable) {
                        activeConnections.add(ip);
                        logMessage("✅ Aktiv gefunden: " + ip);
                    }
                    
                    updateStatus("🔍 " + progress + "/" + generatedIPs.size() + " getestet - " + activeConnections.size() + " aktiv");
                    
                    if (progress >= generatedIPs.size()) {
                        long duration = (System.currentTimeMillis() - startTime) / 1000;
                        completeRangeTest(duration);
                    }
                });
            });
        }
    }
    
    private void completeRangeTest(long duration) {
        isRangeTesting = false;
        progressBar.setVisible(false);
        testButton.setText("🔍 Test ersten IP");
        testButton.setEnabled(true);
        
        StringBuilder result = new StringBuilder();
        result.append("🎯 Range-Test abgeschlossen!\n\n");
        result.append("📊 Ergebnis: ").append(activeConnections.size()).append("/").append(generatedIPs.size()).append(" IPs aktiv\n");
        result.append("⏱️ Dauer: ").append(duration).append("s\n");
        result.append("🎲 Seed: '").append(currentSeed).append("'\n\n");
        
        if (!activeConnections.isEmpty()) {
            result.append("✅ Aktive IPs:\n");
            for (int i = 0; i < activeConnections.size(); i++) {
                result.append(String.format("  %d. %s\n", i + 1, activeConnections.get(i)));
            }
        } else {
            result.append("❌ Keine aktiven IPs gefunden\n");
        }
        
        resultsArea.setText(result.toString());
        updateStatus("✅ Range-Test: " + activeConnections.size() + "/" + generatedIPs.size() + " IPs aktiv");
        
        logMessage("🏁 Range-Test abgeschlossen - " + activeConnections.size() + " aktive Verbindungen");
    }
    
    private boolean testConnection(String ip) {
        try {
            Socket socket = new Socket();
            socket.connect(new InetSocketAddress(ip, 8080), 2000);
            socket.close();
            return true;
        } catch (Exception e) {
            return false;
        }
    }
    
    private void clearSeed() {
        seedInput.setText("");
        currentSeed = "";
        generatedIPs.clear();
        activeConnections.clear();
        
        testButton.setEnabled(false);
        testButton.setText("🔍 Test (Seed erforderlich)");
        progressBar.setVisible(false);
        
        resultsArea.setText("🎯 Hier erscheinen die generierten IPs und Test-Ergebnisse...");
        updateStatus("🧹 Seed gelöscht - bereit für neuen Seed");
        logMessage("🧹 Alle Daten gelöscht");
    }
    
    private void generateRandomSeed() {
        String[] words = {"stellar", "quantum", "nexus", "cipher", "matrix", "beacon", 
                         "synapse", "vertex", "prism", "zenith", "aurora", "cosmos"};
        String randomSeed = words[new Random().nextInt(words.length)] + "-" + (1000 + new Random().nextInt(9000));
        
        seedInput.setText(randomSeed);
        updateStatus("🎲 Zufälliger Seed generiert: " + randomSeed);
        logMessage("🎲 Zufälliger Seed: " + randomSeed);
    }
    
    private void updateStatus(String message) {
        statusLabel.setText("📱 " + message);
    }
    
    private void logMessage(String message) {
        String timestamp = java.time.LocalTime.now().toString().substring(0, 8);
        statusArea.append("[" + timestamp + "] " + message + "\n");
        statusArea.setCaretPosition(statusArea.getDocument().getLength());
    }
    
    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                new EnigmaChat_Desktop_Enhanced().setVisible(true);
            }
        });
    }
}
