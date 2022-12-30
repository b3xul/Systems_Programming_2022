#include <iostream>

//.h
class Fraction {
  int _num, _den;
 public:
  Fraction(int num = 0, int den = 1) : _num(num), _den(den) {
  }
  // trivial because we don't manage dynamic memory
  Fraction(const Fraction &src) : _num(src._num), _den(src._den) {
  };

  explicit operator float() const {
    return (float) _num / (float) _den;
  }

  bool operator()() const { // oggetto funzionale puo' essere sfruttato per effettuare controlli
    return (_den == 0);
  }

  bool operator==(const Fraction &r) const { // anche questa poteva essere fatta come l'operatore + come friend.
    //return ((float)_num/(float)_den == (float)r._num/(float)r._den); duplicazione di codice!
    return (static_cast<float>(*this) == static_cast<float>(r)); // sfrutto l'operatore di conversione ridefinito
  }

  bool operator!=(const Fraction &r) const {
    return !(*this == r); // richiama l'operatore == sulle 2 frazioni *this e r, e inverte il risultato: codice piu'
    // riutilizzabile!
  }

  // altra versione puo' ritornare void invece di Fraction &
  Fraction &operator+=(const Fraction &r) {
    /*_num = r._den * _num + r._num * _den;
      _den = _den * r._den; duplicazione di codice! */
    *this = *this + r; //Sfrutto operatore di somma ridefinito
    return *this;
  }

  friend Fraction operator+(const Fraction &l, const Fraction &r);
  friend std::ostream &operator<<(std::ostream &out, const Fraction &to_print); // in alternativa si puo' definire
  // funzione esternamente, non friend, ma che accede ai campi interni a Fraction usando i getter. Si usa una
  // funzione template che viene istanziata solo nel momento in cui ne viene richiesto l'utilizzo
};

//.cpp
Fraction operator+(const Fraction &l, const Fraction &r) {
  return Fraction(r._den * l._num + r._num * l._den, l._den * r._den); // friend e' necessario per poter accedere
  // ai campi privati di Fraction
}

// N.B. il paramatro out non e' CONST reference!
std::ostream &operator<<(std::ostream &out, const Fraction &to_print) {
  // const reference friend puo' accedere anche ai campi privati!
  out << to_print._num << '/' << to_print._den;
  return out;
}

int main() {
  Fraction a(2, 5);
  Fraction b(5, 6);
  Fraction c = a + b;

  // float var_f = c; Da' errore se convertitore e' marcato come explicit
  float var_f = static_cast<float>(c);
  //auto var_f = static_cast<float>(c); puo' essere utile dato che il tipo viene gia' definito a destra, per
  // ottimizzare il lavoro del compilatore

  std::cout << "Frazione: " << c << '\n';
  if (c()) {
    std::cout << "Il numero ha un denominatore == 0\n";
  }
  return 0;
}
