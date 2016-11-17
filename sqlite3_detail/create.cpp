//
// Created by David Seery on 11/08/2015.
// Copyright (c) 2015 University of Sussex. All rights reserved.
//


#include "utilities.h"

#include "create.h"


namespace sqlite3_operations
  {
    
    namespace create_impl
      {
        
        void wavenubmer_config_table(sqlite3* db, const std::string& table_name, const sqlite3_policy& policy)
          {
            std::ostringstream config_stmt;
            config_stmt
              << "CREATE TABLE " << table_name << "("
              << "id INTEGER PRIMARY KEY, "
              << "k DOUBLE);";
            exec(db, config_stmt.str());
          }
    
        void oneloop_momentum_integral_table(sqlite3* db, const std::string& table_name, const sqlite3_policy& policy)
          {
            std::ostringstream stmt;
            stmt
              << "CREATE TABLE " << table_name << "("
              << "mid INTEGER, "
              << "kid INTEGER, "
              << "IR_id INTEGER, "
              << "UV_id INTEGER, "
              << "value DOUBLE, "
              << "regions DOUBLE, "
              << "evals DOUBLE, "
              << "err DOUBLE, "
              << "time DOUBLE"
#ifdef LSSEFT_STRICT_DATABASE_CONSISTENCY
              << ", "
              << "PRIMARY KEY (mid, kid, IR_id, UV_id), "
              << "FOREIGN KEY (mid) REFERENCES " << policy.FRW_model_table() << "(id), "
              << "FOREIGN KEY (kid) REFERENCES " << policy.wavenumber_config_table() << "(id), "
              << "FOREIGN KEY (IR_id) REFERENCES " << policy.IR_config_table() << "(id), "
              << "FOREIGN KEY (UV_id) REFERENCES " << policy.UV_config_table() << "(id));";
#else
              << ");";
#endif

            exec(db, stmt.str());
          }
        
        
        void oneloop_Pk_table(sqlite3* db, const std::string& table_name, const sqlite3_policy& policy)
          {
            std::ostringstream stmt;
            stmt
              << "CREATE TABLE " << table_name << "("
              << "mid INTEGER, "
              << "zid INTEGER, "
              << "kid INTEGER, "
              << "IR_id INTEGER, "
              << "UV_id INTEGER, "
              << "Ptree DOUBLE, "
              << "err_tree DOUBLE, "
              << "P13 DOUBLE, "
              << "err_13 DOUBLE, "
              << "P22 DOUBLE, "
              << "err_22 DOUBLE, "
              << "P1loopSPT DOUBLE, "
              << "err_1loopSPT DOUBLE, "
              << "Z2_delta DOUBLE"
#ifdef LSSEFT_STRICT_DATABASE_CONSISTENCY
              << ", "
              << "PRIMARY KEY (mid, zid, kid, IR_id, UV_id), "
              << "FOREIGN KEY (mid) REFERENCES " << policy.FRW_model_table() << "(id), "
              << "FOREIGN KEY (kid) REFERENCES " << policy.wavenumber_config_table() << "(id), "
              << "FOREIGN KEY (zid) REFERENCES " << policy.redshift_config_table() << "(id), "
              << "FOREIGN KEY (IR_id) REFERENCES " << policy.IR_config_table() << "(id), "
              << "FOREIGN KEY (UV_id) REFERENCES " << policy.UV_config_table() << "(id));";
#else
              << ");";
#endif
    
            exec(db, stmt.str());
          }
        
        
        void oneloop_rsd_Pk_table(sqlite3* db, const std::string& table_name, const sqlite3_policy& policy)
          {
            std::ostringstream stmt;
            stmt
              << "CREATE TABLE " << table_name << "("
              << "mid INTEGER, "
              << "zid INTEGER, "
              << "kid INTEGER, "
              << "IR_id INTEGER, "
              << "UV_id INTEGER, "
              << "Ptree DOUBLE, "
              << "err_tree DOUBLE, "
              << "P13 DOUBLE, "
              << "err_13 DOUBLE, "
              << "P22 DOUBLE, "
              << "err_22 DOUBLE, "
              << "P1loopSPT DOUBLE, "
              << "err_1loopSPT DOUBLE, "
              << "Z2_delta DOUBLE, "
              << "Z0_v DOUBLE, "
              << "Z2_v DOUBLE, "
              << "Z0_vdelta DOUBLE, "
              << "Z2_vdelta DOUBLE, "
              << "Z2_vv DOUBLE, "
              << "Z2_vvdelta DOUBLE, "
              << "Z2_vvv DOUBLE"
#ifdef LSSEFT_STRICT_DATABASE_CONSISTENCY
              << ", "
              << "PRIMARY KEY (mid, zid, kid, IR_id, UV_id), "
              << "FOREIGN KEY (mid) REFERENCES " << policy.FRW_model_table() << "(id), "
              << "FOREIGN KEY (kid) REFERENCES " << policy.wavenumber_config_table() << "(id), "
              << "FOREIGN KEY (zid) REFERENCES " << policy.redshift_config_table() << "(id), "
              << "FOREIGN KEY (IR_id) REFERENCES " << policy.IR_config_table() << "(id), "
              << "FOREIGN KEY (UV_id) REFERENCES " << policy.UV_config_table() << "(id));";
#else
              << ");";
#endif
    
            exec(db, stmt.str());
          }
        
        
        void multipole_Pk_table(sqlite3* db, const std::string& table_name, const sqlite3_policy& policy)
          {
            std::ostringstream stmt;
            stmt
              << "CREATE TABLE " << table_name << "("
              << "mid INTEGER, "
              << "zid INTEGER, "
              << "kid INTEGER, "
              << "IR_cutoff_id INTEGER, "
              << "UV_cutoff_id INTEGER, "
              << "IR_resum_id INTEGER, "
              << "Ptree DOUBLE, "
              << "Ptree_resum DOUBLE, "
              << "P13 DOUBLE, "
              << "P13_resum DOUBLE, "
              << "P22 DOUBLE, "
              << "P22_resum DOUBLE, "
              << "P1loopSPT DOUBLE, "
              << "P1loopSPT_resum DOUBLE, "
              << "Z2_delta DOUBLE, "
              << "Z0_v DOUBLE, "
              << "Z2_v DOUBLE, "
              << "Z0_vdelta DOUBLE, "
              << "Z2_vdelta DOUBLE, "
              << "Z2_vv DOUBLE, "
              << "Z2_vvdelta DOUBLE, "
              << "Z2_vvv DOUBLE"
#ifdef LSSEFT_STRICT_DATABASE_CONSISTENCY
              << ", "
              << "PRIMARY KEY (mid, zid, kid, IR_cutoff_id, UV_cutoff_id, IR_resum_id), "
              << "FOREIGN KEY (mid) REFERENCES " << policy.FRW_model_table() << "(id), "
              << "FOREIGN KEY (kid) REFERENCES " << policy.wavenumber_config_table() << "(id), "
              << "FOREIGN KEY (zid) REFERENCES " << policy.redshift_config_table() << "(id), "
              << "FOREIGN KEY (IR_cutoff_id) REFERENCES " << policy.IR_config_table() << "(id), "
              << "FOREIGN KEY (UV_cutoff_id) REFERENCES " << policy.UV_config_table() << "(id) "
              << "FOREIGN KEY (IR_resum_id) REFERENCES " << policy.IR_resum_table() << "(id));";
#else
              << ");";
#endif
                                               
            exec(db, stmt.str());
          }
    
      }

    void create_tables(sqlite3* db, const sqlite3_policy& policy)
      {
        std::ostringstream models_stmt;
        models_stmt
          << "CREATE TABLE " << policy.FRW_model_table() << "("
          << "id INTEGER PRIMARY KEY, "
          << "omega_m DOUBLE, "
          << "omega_cc DOUBLE, "
          << "h DOUBLE, "
          << "T_CMB DOUBLE, "
          << "Neff DOUBLE"
          << ");";

        exec(db, models_stmt.str());
        
        std::ostringstream z_config_stmt;
        z_config_stmt
          << "CREATE TABLE " << policy.redshift_config_table() << "("
          << "id INTEGER PRIMARY KEY, "
          << "z DOUBLE);";

        exec(db, z_config_stmt.str());
    
        create_impl::wavenubmer_config_table(db, policy.wavenumber_config_table(), policy);
        create_impl::wavenubmer_config_table(db, policy.IR_config_table(), policy);
        create_impl::wavenubmer_config_table(db, policy.UV_config_table(), policy);
        create_impl::wavenubmer_config_table(db, policy.IR_resum_table(), policy);

        std::ostringstream transfer_stmt;
        transfer_stmt
          << "CREATE TABLE " << policy.transfer_table() << "("
          << "mid INTEGER, "
          << "kid INTEGER, "
          << "zid INTEGER, "
          << "delta_m DOUBLE, "
          << "delta_r DOUBLE, "
          << "theta_m DOUBLE, "
          << "theta_r DOUBLE, "
          << "Phi DOUBLE"
#ifdef LSSEFT_STRICT_DATABASE_CONSISTENCY
          << ", "
          << "PRIMARY KEY (zid, kid), "
          << "FOREIGN KEY (mid) REFERENCES " << policy.FRW_model_table() << "(id), "
          << "FOREIGN KEY (zid) REFERENCES " << policy.redshift_config_table() << "(id), "
          << "FOREIGN KEY (kid) REFERENCES " << policy.wavenumber_config_table() << "(id));";
#else
          << ");";
#endif

        exec(db, transfer_stmt.str());

        std::ostringstream oneloop_g_stmt;
        oneloop_g_stmt
          << "CREATE TABLE " << policy.g_factor_table() << "("
          << "mid INTEGER, "
          << "zid INTEGER, "
          << "g_linear DOUBLE, "
          << "A DOUBLE, "
          << "B DOUBLE, "
          << "D DOUBLE, "
          << "E DOUBLE, "
          << "F DOUBLE, "
          << "G DOUBLE, "
          << "J DOUBLE"
#ifdef LSSEFT_STRICT_DATABASE_CONSISTENCY
          << ", "
          << "PRIMARY KEY (mid, zid), "
          << "FOREIGN KEY (mid) REFERENCES " << policy.FRW_model_table() << "(id), "
          << "FOREIGN KEY (zid) REFERENCES " << policy.redshift_config_table() << "(id));";
#else
          << ");";
#endif

        exec(db, oneloop_g_stmt.str());
    
        std::ostringstream oneloop_f_stmt;
        oneloop_f_stmt
          << "CREATE TABLE " << policy.f_factor_table() << "("
          << "mid INTEGER, "
          << "zid INTEGER, "
          << "f_linear DOUBLE, "
          << "fA DOUBLE, "
          << "fB DOUBLE, "
          << "fD DOUBLE, "
          << "fE DOUBLE, "
          << "fF DOUBLE, "
          << "fG DOUBLE, "
          << "fJ DOUBLE"
#ifdef LSSEFT_STRICT_DATABASE_CONSISTENCY
          << ", "
          << "PRIMARY KEY (mid, zid), "
          << "FOREIGN KEY (mid) REFERENCES " << policy.FRW_model_table() << "(id), "
          << "FOREIGN KEY (zid) REFERENCES " << policy.redshift_config_table() << "(id));";
#else
          << ");";
#endif
    
        exec(db, oneloop_f_stmt.str());
    
        std::ostringstream oneloop_fgmeta_stmt;
        oneloop_fgmeta_stmt
          << "CREATE TABLE " << policy.gf_metadata_table() << " ("
          << "mid INTEGER PRIMARY KEY, "
          << "time INTEGER, "
          << "steps INTEGER, "
          << "FOREIGN KEY (mid) REFERENCES " << policy.FRW_model_table() << "(id));";
        
        exec(db, oneloop_fgmeta_stmt.str());
        
        create_impl::oneloop_momentum_integral_table(db, policy.AA_table(), policy);
        create_impl::oneloop_momentum_integral_table(db, policy.AB_table(), policy);
        create_impl::oneloop_momentum_integral_table(db, policy.BB_table(), policy);

        create_impl::oneloop_momentum_integral_table(db, policy.D_table(), policy);
        create_impl::oneloop_momentum_integral_table(db, policy.E_table(), policy);
        create_impl::oneloop_momentum_integral_table(db, policy.F_table(), policy);
        create_impl::oneloop_momentum_integral_table(db, policy.G_table(), policy);
        create_impl::oneloop_momentum_integral_table(db, policy.J1_table(), policy);
        create_impl::oneloop_momentum_integral_table(db, policy.J2_table(), policy);
        
        create_impl::oneloop_momentum_integral_table(db, policy.RSD13_a_table(), policy);
        create_impl::oneloop_momentum_integral_table(db, policy.RSD13_b_table(), policy);
        create_impl::oneloop_momentum_integral_table(db, policy.RSD13_c_table(), policy);
        create_impl::oneloop_momentum_integral_table(db, policy.RSD13_d_table(), policy);
        create_impl::oneloop_momentum_integral_table(db, policy.RSD13_e_table(), policy);
        create_impl::oneloop_momentum_integral_table(db, policy.RSD13_f_table(), policy);
        create_impl::oneloop_momentum_integral_table(db, policy.RSD13_g_table(), policy);
    
        create_impl::oneloop_momentum_integral_table(db, policy.RSD22_A1_table(), policy);
        create_impl::oneloop_momentum_integral_table(db, policy.RSD22_A2_table(), policy);
        create_impl::oneloop_momentum_integral_table(db, policy.RSD22_A3_table(), policy);
        create_impl::oneloop_momentum_integral_table(db, policy.RSD22_A4_table(), policy);
        create_impl::oneloop_momentum_integral_table(db, policy.RSD22_A5_table(), policy);
        create_impl::oneloop_momentum_integral_table(db, policy.RSD22_B2_table(), policy);
        create_impl::oneloop_momentum_integral_table(db, policy.RSD22_B3_table(), policy);
        create_impl::oneloop_momentum_integral_table(db, policy.RSD22_B6_table(), policy);
        create_impl::oneloop_momentum_integral_table(db, policy.RSD22_B8_table(), policy);
        create_impl::oneloop_momentum_integral_table(db, policy.RSD22_B9_table(), policy);
        create_impl::oneloop_momentum_integral_table(db, policy.RSD22_C1_table(), policy);
        create_impl::oneloop_momentum_integral_table(db, policy.RSD22_C2_table(), policy);
        create_impl::oneloop_momentum_integral_table(db, policy.RSD22_C4_table(), policy);
        create_impl::oneloop_momentum_integral_table(db, policy.RSD22_D1_table(), policy);
    
        create_impl::oneloop_Pk_table(db, policy.dd_Pk_table(), policy);
    
        create_impl::oneloop_rsd_Pk_table(db, policy.dd_rsd_mu0_Pk_table(), policy);
        create_impl::oneloop_rsd_Pk_table(db, policy.dd_rsd_mu2_Pk_table(), policy);
        create_impl::oneloop_rsd_Pk_table(db, policy.dd_rsd_mu4_Pk_table(), policy);
        create_impl::oneloop_rsd_Pk_table(db, policy.dd_rsd_mu6_Pk_table(), policy);
        create_impl::oneloop_rsd_Pk_table(db, policy.dd_rsd_mu8_Pk_table(), policy);
        
        create_impl::multipole_Pk_table(db, policy.P0_table(), policy);
        create_impl::multipole_Pk_table(db, policy.P2_table(), policy);
        create_impl::multipole_Pk_table(db, policy.P4_table(), policy);
      }

  }   // namespace sqlite3_operations
