#!/bin/bash
# 🎮 Test GPU/NPU Detection - Vergleich Console vs. GUI

echo "=== 🎮 GPU DETECTION TEST ==="
echo ""

# 1. GPU Engine Busy (beste Methode)
echo "1️⃣ GPU Engine Busy (direkt %):"
find /sys/class/drm -name busy 2>/dev/null -exec sh -c 'echo "  $(basename $(dirname {})): $(cat {})%"' \;
echo ""

# 2. GPU Frequenz (normalisiert)
echo "2️⃣ GPU Frequenz (MHz):"
if [ -f /sys/class/drm/card0/gt_cur_freq_mhz ]; then
    CUR=$(cat /sys/class/drm/card0/gt_cur_freq_mhz)
    MIN=$(cat /sys/class/drm/card0/gt_min_freq_mhz 2>/dev/null || echo "0")
    MAX=$(cat /sys/class/drm/card0/gt_max_freq_mhz 2>/dev/null || echo "0")
    echo "  Current: ${CUR} MHz"
    echo "  Min: ${MIN} MHz"
    echo "  Max: ${MAX} MHz"
    if [ "$MAX" -gt 0 ] && [ "$MIN" -ge 0 ]; then
        LOAD=$(( (CUR - MIN) * 100 / (MAX - MIN) ))
        echo "  📊 Normalisiert: ${LOAD}%"
    fi
else
    echo "  ❌ Keine GPU-Frequenz verfügbar"
fi
echo ""

# 3. NPU/VPU Detection
echo "=== 🧠 NPU DETECTION TEST ==="
echo ""

echo "1️⃣ Intel VPU Device:"
VPU_FOUND=0
if ls /sys/class/misc/intel_vpu* 2>/dev/null 1>&2; then
    for vpu in /sys/class/misc/intel_vpu*; do
        echo "  ✅ Device: $(basename $vpu)"
        if [ -f "$vpu/device/power_state" ]; then
            STATE=$(cat "$vpu/device/power_state" 2>/dev/null)
            echo "     Power: $STATE"
            if [ "$STATE" != "D3" ] && [ "$STATE" != "off" ]; then
                echo "     📊 NPU aktiv (75%)"
                VPU_FOUND=1
            fi
        fi
    done
else
    echo "  ❌ Kein Intel VPU gefunden"
fi
echo ""

echo "2️⃣ Prozess CPU-Last (FileDuper):"
CPU_USAGE=$(ps aux | grep FileDuper | grep -v grep | awk '{sum+=$3} END {print sum}')
if [ -n "$CPU_USAGE" ]; then
    echo "  CPU: ${CPU_USAGE}%"
    CPU_INT=$(printf "%.0f" "$CPU_USAGE")
    if [ "$CPU_INT" -lt 30 ] && pgrep -x FileDuper > /dev/null; then
        echo "  📊 NPU entlastet CPU (60%)"
    fi
else
    echo "  ❌ FileDuper nicht aktiv"
fi
echo ""

echo "3️⃣ Kernel Messages (dmesg):"
if dmesg 2>/dev/null | grep -qi "vpu\|npu"; then
    echo "  ✅ VPU/NPU im Kernel erkannt"
    dmesg | grep -i "vpu\|npu" | tail -3
else
    echo "  ❌ Keine VPU/NPU Kernel-Messages"
fi
echo ""

# Summary
echo "=== 📊 ERWARTETE GUI-WERTE ==="
echo ""
echo "🎮 GPU:"
find /sys/class/drm -name busy 2>/dev/null | head -1 | xargs cat | awk '{print "  Sollte zeigen: " $1 "%"}'
echo ""
echo "🧠 NPU:"
if [ "$VPU_FOUND" -eq 1 ]; then
    echo "  Sollte zeigen: 50-75% (lila)"
else
    echo "  Sollte zeigen: Inaktiv (grau)"
fi
echo ""

echo "🔄 GUI Live-Updates:"
echo "  - Alle 250ms (4x pro Sekunde)"
echo "  - Farben: Grau (0) → Grün (1-30%) → Gelb (30-70%) → Orange (70-90%) → Rot (90%+)"
echo "  - NPU: Lila (#8B5CF6) wenn aktiv"
