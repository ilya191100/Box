uint8_t getFingerprintEnroll() {

  int p = -1;
  if(debug)Serial.print("Waiting for valid finger to enroll as #"); if(debug)Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      if(debug)Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      if(debug)Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      if(debug)Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      if(debug)Serial.println("Imaging error");
      break;
    default:
      if(debug)Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      if(debug)Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      if(debug)Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      if(debug)Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      if(debug)Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      if(debug)Serial.println("Could not find fingerprint features");
      return p;
    default:
      if(debug)Serial.println("Unknown error");
      return p;
  }
  
  if(debug)Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  if(debug)Serial.print("ID "); Serial.println(id);
  p = -1;
  if(debug)Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      if(debug)Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      if(debug)Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      if(debug)Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      if(debug)Serial.println("Imaging error");
      break;
    default:
      if(debug)Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      if(debug)Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      if(debug)Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      if(debug)Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      if(debug)Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      if(debug)Serial.println("Could not find fingerprint features");
      return p;
    default:
      if(debug)Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  if(debug)Serial.print("Creating model for #");  if(debug)Serial.println(id);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    if(debug)Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    if(debug)Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    if(debug)Serial.println("Fingerprints did not match");
    return p;
  } else {
    if(debug)Serial.println("Unknown error");
    return p;
  }   
  
  if(debug)Serial.print("ID "); if(debug)Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    if(debug)Serial.println("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    if(debug)Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    if(debug)Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    if(debug)Serial.println("Error writing to flash");
    return p;
  } else {
    if(debug)Serial.println("Unknown error");
    return p;
  }   
}
