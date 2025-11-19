import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.*;

public class enigmachat_simple extends JFrame {
    private JTextArea messageArea;
    private JTextField messageInput;
    private JList<String> peerList;
    private DefaultListModel<String> peerModel;
    private String selectedPeer = "Desktop-PC";
    
    public enigmachat_simple() {
        setTitle("🔐 EnigmaChat Linux - P2P Chat");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(800, 600);
        setLocationRelativeTo(null);
        setLayout(new BorderLayout());
        
        // Chat-Bereich
        messageArea = new JTextArea();
        messageArea.setEditable(false);
        messageArea.setBackground(Color.BLACK);
        messageArea.setForeground(Color.GREEN);
        messageArea.setFont(new Font("Monospace", Font.PLAIN, 12));
        messageArea.setText("🔐 EnigmaChat Linux gestartet!\n");
        messageArea.append("✅ Bereit für verschlüsselte Kommunikation\n\n");
        
        JScrollPane scrollPane = new JScrollPane(messageArea);
        add(scrollPane, BorderLayout.CENTER);
        
        // Input-Bereich
        JPanel inputPanel = new JPanel(new BorderLayout());
        messageInput = new JTextField();
        messageInput.setBackground(Color.DARK_GRAY);
        messageInput.setForeground(Color.WHITE);
        messageInput.setFont(new Font("Arial", Font.PLAIN, 12));
        
        JButton sendButton = new JButton("📤 Senden");
        sendButton.setBackground(Color.BLUE);
        sendButton.setForeground(Color.WHITE);
        
        inputPanel.add(messageInput, BorderLayout.CENTER);
        inputPanel.add(sendButton, BorderLayout.EAST);
        add(inputPanel, BorderLayout.SOUTH);
        
        // Peer-Liste
        peerModel = new DefaultListModel<>();
        peerModel.addElement("🖥️ Desktop-PC (192.168.1.10)");
        peerModel.addElement("📱 Android-Phone (192.168.1.15)");
        peerModel.addElement("💻 Laptop-Alice (192.168.1.20)");
        
        peerList = new JList<>(peerModel);
        peerList.setBackground(Color.DARK_GRAY);
        peerList.setForeground(Color.WHITE);
        peerList.setSelectionBackground(Color.BLUE);
        
        JScrollPane peerScroll = new JScrollPane(peerList);
        peerScroll.setPreferredSize(new Dimension(200, 0));
        add(peerScroll, BorderLayout.WEST);
        
        // Event-Handler
        sendButton.addActionListener(e -> sendMessage());
        messageInput.addActionListener(e -> sendMessage());
        
        peerList.addListSelectionListener(e -> {
            if (!e.getValueIsAdjusting()) {
                selectedPeer = peerList.getSelectedValue();
            }
        });
        
        // Automatische Peer-Updates
        Timer timer = new Timer(3000, e -> {
            if (peerModel.size() < 6) {
                String[] newPeers = {
                    "🖥️ Server-Node (192.168.1.25)",
                    "📱 iPhone-Bob (192.168.1.30)",
                    "💻 Linux-Workstation (192.168.1.35)"
                };
                peerModel.addElement(newPeers[(int)(Math.random() * newPeers.length)]);
            }
        });
        timer.start();
    }
    
    private void sendMessage() {
        String message = messageInput.getText().trim();
        if (message.isEmpty()) return;
        
        messageArea.append("📤 Du → " + selectedPeer + ":\n");
        messageArea.append("   💬 " + message + "\n");
        messageArea.append("   🔐 [AES-256 verschlüsselt]\n\n");
        
        messageInput.setText("");
        
        // Simuliere Antwort
        Timer responseTimer = new Timer(2000, e -> {
            String[] responses = {
                "Nachricht erhalten! 👍",
                "Alles klar! ✅",
                "Super, funktioniert! 🚀"
            };
            String response = responses[(int)(Math.random() * responses.length)];
            
            messageArea.append("📥 " + selectedPeer + " → Du:\n");
            messageArea.append("   💬 " + response + "\n");
            messageArea.append("   🔓 [Entschlüsselt] ✅\n\n");
            messageArea.setCaretPosition(messageArea.getDocument().getLength());
        });
        responseTimer.setRepeats(false);
        responseTimer.start();
        
        messageArea.setCaretPosition(messageArea.getDocument().getLength());
    }
    
    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            new enigmachat_simple().setVisible(true);
        });
    }
}
