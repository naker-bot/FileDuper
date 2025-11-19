import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.net.*;
import java.io.*;
import java.util.*;
import java.util.List;
import java.util.concurrent.*;

public class enigmachat_real_network extends JFrame {
    private JTextArea messageArea;
    private JTextField messageInput;
    private JList<String> peerList;
    private DefaultListModel<String> peerModel;
    private String selectedPeer = null;
    private Set<String> discoveredDevices;
    private ExecutorService executor;
    private boolean isScanning = false;
    
    public enigmachat_real_network() {
        discoveredDevices = new HashSet<>();
        executor = Executors.newFixedThreadPool(10);
        initGUI();
        startRealNetworkDiscovery();
    }
    
    private void initGUI() {
        setTitle("🔍 EnigmaChat - Echte Netzwerk-Discovery");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(900, 700);
        setLocationRelativeTo(null);
        setLayout(new BorderLayout());
        
        // Chat-Bereich
        messageArea = new JTextArea();
        messageArea.setEditable(false);
        messageArea.setBackground(Color.BLACK);
        messageArea.setForeground(Color.GREEN);
        messageArea.setFont(new Font("Monospace", Font.PLAIN, 11));
        messageArea.setText("🔍 EnigmaChat - Echte Netzwerk-Discovery gestartet!\n");
        messageArea.append("🌐 Scanne lokales Netzwerk nach echten Geräten...\n\n");
        
        JScrollPane scrollPane = new JScrollPane(messageArea);
        add(scrollPane, BorderLayout.CENTER);
        
        // Peer-Liste
        peerModel = new DefaultListModel<>();
        peerList = new JList<>(peerModel);
        peerList.setBackground(Color.DARK_GRAY);
        peerList.setForeground(Color.WHITE);
        peerList.setSelectionBackground(Color.BLUE);
        peerList.setFont(new Font("Monospace", Font.PLAIN, 10));
        
        peerList.addListSelectionListener(e -> {
            if (!e.getValueIsAdjusting()) {
                selectedPeer = peerList.getSelectedValue();
                if (selectedPeer != null) {
                    messageArea.append("✅ Ausgewähltes Gerät: " + selectedPeer + "\n\n");
                    messageArea.setCaretPosition(messageArea.getDocument().getLength());
                }
            }
        });
        
        JPanel leftPanel = new JPanel(new BorderLayout());
        leftPanel.add(new JLabel("🌐 Gefundene Netzwerk-Geräte:"), BorderLayout.NORTH);
        leftPanel.add(new JScrollPane(peerList), BorderLayout.CENTER);
        leftPanel.setPreferredSize(new Dimension(300, 0));
        add(leftPanel, BorderLayout.WEST);
        
        // Input-Bereich
        JPanel inputPanel = new JPanel(new BorderLayout());
        messageInput = new JTextField();
        messageInput.setBackground(Color.DARK_GRAY);
        messageInput.setForeground(Color.WHITE);
        
        JButton sendButton = new JButton("📤 Senden");
        sendButton.setBackground(Color.BLUE);
        sendButton.setForeground(Color.WHITE);
        
        JButton scanButton = new JButton("🔄 Netzwerk scannen");
        scanButton.setBackground(Color.ORANGE);
        scanButton.setForeground(Color.WHITE);
        
        sendButton.addActionListener(e -> sendMessage());
        messageInput.addActionListener(e -> sendMessage());
        scanButton.addActionListener(e -> startRealNetworkDiscovery());
        
        JPanel buttonPanel = new JPanel(new FlowLayout());
        buttonPanel.add(scanButton);
        buttonPanel.add(sendButton);
        
        inputPanel.add(new JLabel("💬 Nachricht: "), BorderLayout.WEST);
        inputPanel.add(messageInput, BorderLayout.CENTER);
        inputPanel.add(buttonPanel, BorderLayout.EAST);
        add(inputPanel, BorderLayout.SOUTH);
        
        // Status-Panel
        JPanel statusPanel = new JPanel(new FlowLayout());
        statusPanel.setBackground(Color.DARK_GRAY);
        statusPanel.add(new JLabel("🔍 Status: Scanne Netzwerk..."));
        add(statusPanel, BorderLayout.NORTH);
    }
    
    private void startRealNetworkDiscovery() {
        if (isScanning) {
            messageArea.append("⚠️ Scan läuft bereits...\n\n");
            return;
        }
        
        isScanning = true;
        peerModel.clear();
        discoveredDevices.clear();
        
        messageArea.append("🔍 Starte echten Netzwerk-Scan...\n");
        messageArea.append("🌐 Prüfe lokales Subnetz auf aktive Geräte\n\n");
        messageArea.setCaretPosition(messageArea.getDocument().getLength());
        
        // Erkenne lokale IP und Subnetz
        new Thread(() -> {
            try {
                String localIP = getLocalIP();
                messageArea.append("🏠 Lokale IP: " + localIP + "\n");
                
                String subnet = localIP.substring(0, localIP.lastIndexOf('.'));
                messageArea.append("🌐 Scanne Subnetz: " + subnet + ".1-254\n\n");
                messageArea.setCaretPosition(messageArea.getDocument().getLength());
                
                // Schneller Ping-Scan
                CountDownLatch latch = new CountDownLatch(254);
                
                for (int i = 1; i <= 254; i++) {
                    final String ip = subnet + "." + i;
                    
                    executor.submit(() -> {
                        try {
                            if (pingHost(ip, 1000)) {
                                String deviceInfo = getDeviceInfo(ip);
                                SwingUtilities.invokeLater(() -> {
                                    if (discoveredDevices.add(deviceInfo)) {
                                        peerModel.addElement(deviceInfo);
                                        messageArea.append("🎯 Gerät gefunden: " + deviceInfo + "\n");
                                        messageArea.setCaretPosition(messageArea.getDocument().getLength());
                                    }
                                });
                            }
                        } catch (Exception e) {
                            // Ignoriere Fehler
                        } finally {
                            latch.countDown();
                        }
                    });
                }
                
                // Warte auf Scan-Completion
                new Thread(() -> {
                    try {
                        latch.await(30, TimeUnit.SECONDS);
                        SwingUtilities.invokeLater(() -> {
                            messageArea.append("\n✅ Netzwerk-Scan abgeschlossen!\n");
                            messageArea.append("🌐 " + discoveredDevices.size() + " aktive Geräte gefunden\n\n");
                            messageArea.setCaretPosition(messageArea.getDocument().getLength());
                            isScanning = false;
                        });
                    } catch (InterruptedException e) {
                        isScanning = false;
                    }
                }).start();
                
            } catch (Exception e) {
                SwingUtilities.invokeLater(() -> {
                    messageArea.append("❌ Fehler beim Netzwerk-Scan: " + e.getMessage() + "\n\n");
                    isScanning = false;
                });
            }
        }).start();
    }
    
    private String getLocalIP() throws Exception {
        // Finde lokale IP-Adresse
        try (DatagramSocket socket = new DatagramSocket()) {
            socket.connect(InetAddress.getByName("8.8.8.8"), 80);
            return socket.getLocalAddress().getHostAddress();
        }
    }
    
    private boolean pingHost(String ip, int timeout) {
        try {
            InetAddress address = InetAddress.getByName(ip);
            return address.isReachable(timeout);
        } catch (Exception e) {
            return false;
        }
    }
    
    private String getDeviceInfo(String ip) {
        try {
            InetAddress address = InetAddress.getByName(ip);
            String hostname = address.getHostName();
            
            // Versuche Gerätetype zu ermitteln
            String deviceType = "🖥️";
            if (hostname.toLowerCase().contains("android") || hostname.toLowerCase().contains("phone")) {
                deviceType = "📱";
            } else if (hostname.toLowerCase().contains("laptop") || hostname.toLowerCase().contains("macbook")) {
                deviceType = "💻";
            } else if (hostname.toLowerCase().contains("router") || hostname.toLowerCase().contains("gateway")) {
                deviceType = "📡";
            } else if (hostname.toLowerCase().contains("printer")) {
                deviceType = "🖨️";
            } else if (hostname.toLowerCase().contains("pi") || hostname.toLowerCase().contains("raspberry")) {
                deviceType = "🏠";
            }
            
            String name = hostname.equals(ip) ? "Unknown-Device" : hostname;
            return deviceType + " " + name + " (" + ip + ")";
            
        } catch (Exception e) {
            return "🖥️ Unknown-Device (" + ip + ")";
        }
    }
    
    private void sendMessage() {
        String message = messageInput.getText().trim();
        if (message.isEmpty()) return;
        
        if (selectedPeer == null) {
            JOptionPane.showMessageDialog(this, 
                "❌ Bitte wähle zuerst ein Gerät aus der Liste!\n\n" +
                "🔍 Verfügbare Geräte werden durch Netzwerk-Scan gefunden.", 
                "Kein Gerät ausgewählt", 
                JOptionPane.WARNING_MESSAGE);
            return;
        }
        
        // Extrahiere IP aus selectedPeer
        String targetIP = extractIP(selectedPeer);
        
        messageArea.append("📤 Sende an " + selectedPeer + ":\n");
        messageArea.append("   💬 \"" + message + "\"\n");
        messageArea.append("   🌐 Ziel-IP: " + targetIP + "\n");
        
        // Versuche echte Netzwerk-Verbindung
        executor.submit(() -> {
            try {
                boolean sent = sendRealMessage(targetIP, message);
                SwingUtilities.invokeLater(() -> {
                    if (sent) {
                        messageArea.append("   ✅ Nachricht erfolgreich gesendet!\n\n");
                    } else {
                        messageArea.append("   ❌ Verbindung fehlgeschlagen (Gerät antwortet nicht)\n\n");
                    }
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
    
    private String extractIP(String deviceInfo) {
        int start = deviceInfo.lastIndexOf('(') + 1;
        int end = deviceInfo.lastIndexOf(')');
        return deviceInfo.substring(start, end);
    }
    
    private boolean sendRealMessage(String ip, String message) {
        try {
            // Versuche verschiedene Ports für Verbindung
            int[] ports = {8080, 22, 80, 443, 23, 21, 25, 53};
            
            for (int port : ports) {
                try (Socket socket = new Socket()) {
                    socket.connect(new InetSocketAddress(ip, port), 3000);
                    
                    PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
                    out.println("EnigmaChat: " + message);
                    
                    return true; // Erfolgreich verbunden
                } catch (Exception e) {
                    // Nächsten Port versuchen
                }
            }
            
            return false; // Keine Verbindung möglich
            
        } catch (Exception e) {
            return false;
        }
    }
    
    private void showNetworkInfo() {
        try {
            String localIP = getLocalIP();
            String info = 
                "🌐 Netzwerk-Informationen\n\n" +
                "🏠 Lokale IP: " + localIP + "\n" +
                "🔍 Scan-Bereich: " + localIP.substring(0, localIP.lastIndexOf('.')) + ".1-254\n" +
                "📡 Aktive Geräte: " + discoveredDevices.size() + "\n\n" +
                "ℹ️ EnigmaChat scannt das lokale Netzwerk nach\n" +
                "   echten Geräten und versucht Verbindungen\n" +
                "   über Standard-Ports aufzubauen.";
            
            JOptionPane.showMessageDialog(this, info, 
                "🌐 Netzwerk-Info", JOptionPane.INFORMATION_MESSAGE);
                
        } catch (Exception e) {
            JOptionPane.showMessageDialog(this, 
                "❌ Fehler beim Abrufen der Netzwerk-Info: " + e.getMessage(), 
                "Fehler", JOptionPane.ERROR_MESSAGE);
        }
    }
    
    public static void main(String[] args) {
        System.out.println("🔍 Starte EnigmaChat mit echter Netzwerk-Discovery...");
        
        SwingUtilities.invokeLater(() -> {
            try {
                new enigmachat_real_network().setVisible(true);
                System.out.println("✅ EnigmaChat bereit für echte Netzwerk-Kommunikation!");
            } catch (Exception e) {
                System.err.println("❌ Fehler: " + e.getMessage());
                e.printStackTrace();
            }
        });
    }
}
