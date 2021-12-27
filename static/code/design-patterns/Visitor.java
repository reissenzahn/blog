

interface CreditCard {
  String getName();
  accept(OfferVisitor v);
}

class BronzeCreditCard implements CreditCard {
  public String getName() {
    return "bronze";
  }
}

class SilverCreditCard implements CreditCard {
  public String getName() {
    return "silver";
  }
}

class GoldCreditCard implements CreditCard {
  public String getName() {
    return "gold";
  }
}


interface OfferVisitor {
  void visitBronze(BronzeCreditCard card);
  void visitSilver(SilverCreditCard card);
  void visitGold(GoldCreditCard card);
}

