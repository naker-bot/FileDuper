package com.enigmachat.enhanced

import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.view.View
import android.widget.*
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import com.google.android.material.textfield.TextInputEditText
import com.google.android.material.textfield.TextInputLayout
import java.net.InetSocketAddress
import java.net.Socket
import java.security.MessageDigest
import java.util.concurrent.ExecutorService
import java.util.concurrent.Executors
import java.util.concurrent.atomic.AtomicInteger

class MainActivity : AppCompatActivity() {
    
    private lateinit var seedInputLayout: TextInputLayout
    private lateinit var seedInput: TextInputEditText
    private lateinit var applySeedBtn: Button
    private lateinit var clearSeedBtn: Button
    private lateinit var generateSeedBtn: Button
    private lateinit var testBtn: Button
    private lateinit var progressBar: ProgressBar
    private lateinit var statusText: TextView
    private lateinit var resultsText: TextView
    private lateinit var ipListText: TextView
    
    private var currentSeed: String = ""
    private var generatedIPs: List<String> = emptyList()
    private var isRangeTesting = false
    private val executor: ExecutorService = Executors.newFixedThreadPool(10)
    private val mainHandler = Handler(Looper.getMainLooper())
    
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        
        initViews()
        setupClickListeners()
        updateStatus("🎲 EnigmaChat Enhanced bereit - Seed eingeben")
    }
    
    private fun initViews() {
        seedInputLayout = findViewById(R.id.seedInputLayout)
        seedInput = findViewById(R.id.seedInput)
        applySeedBtn = findViewById(R.id.applySeedBtn)
        clearSeedBtn = findViewById(R.id.clearSeedBtn)
        generateSeedBtn = findViewById(R.id.generateSeedBtn)
        testBtn = findViewById(R.id.testBtn)
        progressBar = findViewById(R.id.progressBar)
        statusText = findViewById(R.id.statusText)
        resultsText = findViewById(R.id.resultsText)
        ipListText = findViewById(R.id.ipListText)
        
        // Initial UI state
        testBtn.isEnabled = false
        progressBar.visibility = View.GONE
        
        // Set default seed
        seedInput.setText("mein-geheimer-schluessel")
    }
    
    private fun setupClickListeners() {
        applySeedBtn.setOnClickListener { applySeed() }
        clearSeedBtn.setOnClickListener { clearSeed() }
        generateSeedBtn.setOnClickListener { generateRandomSeed() }
        
        testBtn.setOnClickListener { testSingleIP() }
        testBtn.setOnLongClickListener { 
            testAllSeedIPs()
            true
        }
    }
    
    private fun applySeed() {
        val seed = seedInput.text.toString().trim()
        if (seed.isEmpty()) {
            updateStatus("❌ Kein Seed eingegeben")
            seedInputLayout.error = "Seed erforderlich"
            return
        }
        
        seedInputLayout.error = null
        currentSeed = seed
        generatedIPs = generateSeedBasedIPs(seed)
        
        updateStatus("✅ Seed '$seed' angewendet - ${generatedIPs.size} IPs generiert")
        displayGeneratedIPs()
        
        testBtn.isEnabled = true
        testBtn.text = "🔍 Test ersten IP"
    }
    
    private fun generateSeedBasedIPs(seed: String): List<String> {
        val ips = mutableListOf<String>()
        
        try {
            val digest = MessageDigest.getInstance("SHA-256")
            val hash = digest.digest(seed.toByteArray())
            
            // Generate 25 deterministic IPs based on hash
            for (i in 0 until 25) {
                val byte3 = (hash[i % hash.size].toInt() and 0xFF) % 255 + 1
                val byte4 = (hash[(i + 1) % hash.size].toInt() and 0xFF) % 254 + 1
                ips.add("192.168.$byte3.$byte4")
            }
            
            // Add some local network IPs
            for (i in 1..10) {
                ips.add("192.168.1.$i")
            }
            
        } catch (e: Exception) {
            // Fallback IPs
            for (i in 1..25) {
                ips.add("192.168.1.$i")
            }
        }
        
        return ips.distinct()
    }
    
    private fun displayGeneratedIPs() {
        val sb = StringBuilder()
        sb.append("🎲 Generierte IPs für Seed '$currentSeed':\n\n")
        
        generatedIPs.forEachIndexed { index, ip ->
            sb.append("${index + 1}. $ip\n")
        }
        
        sb.append("\n💡 Long-Press 'Test' für vollständigen Range-Scan")
        ipListText.text = sb.toString()
    }
    
    private fun testSingleIP() {
        if (generatedIPs.isEmpty()) {
            updateStatus("❌ Erst Seed anwenden")
            return
        }
        
        val testIP = generatedIPs[0]
        updateStatus("🔍 Teste $testIP...")
        progressBar.visibility = View.VISIBLE
        progressBar.isIndeterminate = true
        
        executor.execute {
            val isReachable = testConnection(testIP)
            mainHandler.post {
                progressBar.visibility = View.GONE
                val result = if (isReachable) "✅ $testIP ist erreichbar!" else "❌ $testIP nicht erreichbar"
                updateStatus(result)
                resultsText.text = result
            }
        }
    }
    
    private fun testAllSeedIPs() {
        if (generatedIPs.isEmpty()) {
            updateStatus("❌ Erst Seed anwenden")
            return
        }
        
        if (isRangeTesting) {
            updateStatus("⚠️ Range-Test läuft bereits")
            return
        }
        
        showRangeTestDialog()
    }
    
    private fun showRangeTestDialog() {
        AlertDialog.Builder(this)
            .setTitle("🔍 Vollständiger Range-Test")
            .setMessage("Möchten Sie alle ${generatedIPs.size} IPs testen?\n\nDies kann einige Minuten dauern.")
            .setPositiveButton("🚀 Ja, starten") { _, _ -> 
                startRangeTest()
            }
            .setNegativeButton("❌ Abbrechen", null)
            .show()
    }
    
    private fun startRangeTest() {
        isRangeTesting = true
        val testProgress = AtomicInteger(0)
        val activeConnections = mutableListOf<String>()
        
        updateStatus("🔍 Starte Range-Test für ${generatedIPs.size} IPs...")
        progressBar.visibility = View.VISIBLE
        progressBar.isIndeterminate = false
        progressBar.max = generatedIPs.size
        progressBar.progress = 0
        
        testBtn.text = "⏳ Testing..."
        testBtn.isEnabled = false
        
        val startTime = System.currentTimeMillis()
        
        generatedIPs.forEach { ip ->
            executor.execute {
                val isReachable = testConnection(ip)
                
                mainHandler.post {
                    val progress = testProgress.incrementAndGet()
                    progressBar.progress = progress
                    
                    if (isReachable) {
                        synchronized(activeConnections) {
                            activeConnections.add(ip)
                        }
                    }
                    
                    updateStatus("🔍 $progress/${generatedIPs.size} getestet - ${activeConnections.size} aktiv")
                    
                    if (progress >= generatedIPs.size) {
                        val duration = (System.currentTimeMillis() - startTime) / 1000
                        completeRangeTest(activeConnections.toList(), duration)
                    }
                }
            }
        }
    }
    
    private fun completeRangeTest(activeConnections: List<String>, duration: Long) {
        isRangeTesting = false
        progressBar.visibility = View.GONE
        testBtn.text = "🔍 Test ersten IP"
        testBtn.isEnabled = true
        
        val result = StringBuilder()
        result.append("🎯 Range-Test abgeschlossen!\n\n")
        result.append("📊 Ergebnis: ${activeConnections.size}/${generatedIPs.size} IPs aktiv\n")
        result.append("⏱️ Dauer: ${duration}s\n")
        result.append("🎲 Seed: '$currentSeed'\n\n")
        
        if (activeConnections.isNotEmpty()) {
            result.append("✅ Aktive IPs:\n")
            activeConnections.forEachIndexed { index, ip ->
                result.append("  ${index + 1}. $ip\n")
            }
        } else {
            result.append("❌ Keine aktiven IPs gefunden\n")
        }
        
        resultsText.text = result.toString()
        updateStatus("✅ Range-Test: ${activeConnections.size}/${generatedIPs.size} IPs aktiv")
        
        // Show completion dialog
        AlertDialog.Builder(this)
            .setTitle("🏁 Range-Test abgeschlossen")
            .setMessage("${activeConnections.size} von ${generatedIPs.size} IPs sind aktiv.\n\nDauer: ${duration} Sekunden")
            .setPositiveButton("✅ OK", null)
            .show()
    }
    
    private fun testConnection(ip: String): Boolean {
        return try {
            val socket = Socket()
            socket.connect(InetSocketAddress(ip, 8080), 2000)
            socket.close()
            true
        } catch (e: Exception) {
            false
        }
    }
    
    private fun clearSeed() {
        seedInput.setText("")
        currentSeed = ""
        generatedIPs = emptyList()
        
        testBtn.isEnabled = false
        testBtn.text = "🔍 Test (Seed erforderlich)"
        progressBar.visibility = View.GONE
        
        ipListText.text = "🎯 Hier erscheinen die generierten IPs..."
        resultsText.text = "📊 Test-Ergebnisse erscheinen hier..."
        updateStatus("🧹 Seed gelöscht - bereit für neuen Seed")
        
        seedInputLayout.error = null
    }
    
    private fun generateRandomSeed() {
        val words = arrayOf("stellar", "quantum", "nexus", "cipher", "matrix", "beacon", 
                           "synapse", "vertex", "prism", "zenith", "aurora", "cosmos")
        val randomSeed = "${words.random()}-${(1000..9999).random()}"
        
        seedInput.setText(randomSeed)
        updateStatus("🎲 Zufälliger Seed generiert: $randomSeed")
    }
    
    private fun updateStatus(message: String) {
        statusText.text = "📱 $message"
    }
    
    override fun onDestroy() {
        super.onDestroy()
        executor.shutdown()
    }
}