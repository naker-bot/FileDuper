// OpenCL SHA256 Hash Kernel für GPU-beschleunigte Hash-Berechnung
// Optimiert für Intel GPU (Arc, Xe, UHD Graphics)

#define ROTR(x, n) (((x) >> (n)) | ((x) << (32 - (n))))

#define CH(x, y, z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22))
#define EP1(x) (ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25))
#define SIG0(x) (ROTR(x, 7) ^ ROTR(x, 18) ^ ((x) >> 3))
#define SIG1(x) (ROTR(x, 17) ^ ROTR(x, 19) ^ ((x) >> 10))

__constant uint K[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

__kernel void sha256_hash(__global const uchar* data, 
                         __global uint* hash, 
                         uint data_len, 
                         uint chunk_size) {
    int gid = get_global_id(0);
    int chunk_start = gid * chunk_size;
    
    if (chunk_start >= data_len) return;
    
    uint actual_chunk_size = min(chunk_size, data_len - chunk_start);
    
    // SHA256 initial hash values
    uint h[8] = {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
        0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
    };
    
    // Process data in 512-bit chunks
    uint w[64];
    uint a, b, c, d, e, f, g, h_temp;
    
    for (uint chunk_offset = 0; chunk_offset < actual_chunk_size; chunk_offset += 64) {
        // Clear w array
        for (int i = 0; i < 64; i++) {
            w[i] = 0;
        }
        
        // Copy chunk into first 16 words of w array
        for (int i = 0; i < 16 && (chunk_offset + i * 4) < actual_chunk_size; i++) {
            uint byte_offset = chunk_start + chunk_offset + i * 4;
            if (byte_offset + 3 < data_len) {
                w[i] = (data[byte_offset] << 24) | 
                       (data[byte_offset + 1] << 16) | 
                       (data[byte_offset + 2] << 8) | 
                       data[byte_offset + 3];
            } else {
                // Handle padding for last chunk
                for (int j = 0; j < 4; j++) {
                    if (byte_offset + j < data_len) {
                        w[i] |= (data[byte_offset + j] << (24 - j * 8));
                    }
                }
            }
        }
        
        // Extend the first 16 words into the remaining 48 words
        for (int i = 16; i < 64; i++) {
            uint s0 = SIG0(w[i - 15]);
            uint s1 = SIG1(w[i - 2]);
            w[i] = w[i - 16] + s0 + w[i - 7] + s1;
        }
        
        // Initialize working variables
        a = h[0]; b = h[1]; c = h[2]; d = h[3];
        e = h[4]; f = h[5]; g = h[6]; h_temp = h[7];
        
        // Main loop
        for (int i = 0; i < 64; i++) {
            uint S1 = EP1(e);
            uint ch = CH(e, f, g);
            uint temp1 = h_temp + S1 + ch + K[i] + w[i];
            uint S0 = EP0(a);
            uint maj = MAJ(a, b, c);
            uint temp2 = S0 + maj;
            
            h_temp = g;
            g = f;
            f = e;
            e = d + temp1;
            d = c;
            c = b;
            b = a;
            a = temp1 + temp2;
        }
        
        // Add compressed chunk to current hash value
        h[0] += a; h[1] += b; h[2] += c; h[3] += d;
        h[4] += e; h[5] += f; h[6] += g; h[7] += h_temp;
    }
    
    // Store result
    for (int i = 0; i < 8; i++) {
        hash[gid * 8 + i] = h[i];
    }
}
