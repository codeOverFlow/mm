/*
 * File: z_en_tab.c
 * Overlay: ovl_En_Tab
 * Description: Talon B - Mr. Barten
 */

#include "z_en_tab.h"
#include "objects/gameplay_keep/gameplay_keep.h"
#include "objects/object_tab/object_tab.h"

#define FLAGS (ACTOR_FLAG_1 | ACTOR_FLAG_8 | ACTOR_FLAG_10 | ACTOR_FLAG_20)

#define THIS ((EnTab*)thisx)

void EnTab_Init(Actor* thisx, PlayState* play);
void EnTab_Destroy(Actor* thisx, PlayState* play);
void EnTab_Update(Actor* thisx, PlayState* play);
void EnTab_Draw(Actor* thisx, PlayState* play);

void func_80BE127C(EnTab* this, PlayState* play);
void func_80BE1348(EnTab* this, PlayState* play);

static u8 D_80BE18D0[] = {
    /* 0x00 */ SCHEDULE_CMD_CHECK_NOT_IN_DAY_S(3, 0x24 - 0x04),
    /* 0x04 */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_MILK_BAR, 0x23 - 0x08),
    /* 0x08 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(9, 50, 18, 1, 0x1D - 0x0E),
    /* 0x0E */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(18, 0, 6, 0, 0x17 - 0x14),
    /* 0x14 */ SCHEDULE_CMD_RET_VAL_L(0),
    /* 0x17 */ SCHEDULE_CMD_RET_TIME(18, 0, 6, 0, 2),
    /* 0x1D */ SCHEDULE_CMD_RET_TIME(9, 50, 18, 1, 1),
    /* 0x23 */ SCHEDULE_CMD_RET_NONE(),
    /* 0x24 */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_MILK_BAR, 0x41 - 0x28),
    /* 0x28 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(9, 50, 21, 5, 0x3B - 0x2E),
    /* 0x2E */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(21, 55, 5, 5, 0x35 - 0x34),
    /* 0x34 */ SCHEDULE_CMD_RET_NONE(),
    /* 0x35 */ SCHEDULE_CMD_RET_TIME(21, 55, 5, 5, 2),
    /* 0x3B */ SCHEDULE_CMD_RET_TIME(9, 50, 21, 5, 1),
    /* 0x41 */ SCHEDULE_CMD_RET_NONE(),
};

s32 D_80BE1914[] = {
    0x003A0200, 0x080E2AF9, 0x0C113A02, 0x100E2AFA, 0x0C150900, 0x000E2AFB,
    0x0C003401, 0x00090F2A, 0xFD0C0F2A, 0xFE0C100F, 0x2AFC0C10,
};

s32 D_80BE1940[] = {
    0x003F0400, 0x080E2B3D, 0x0C113F04, 0x100E2B3E, 0x0C0F2AFA, 0x0C150900,
    0x000E2AFB, 0x0C003401, 0x00090F2A, 0xFD0C0F2A, 0xFE0C100F, 0x2AFC0C10,
};

s32 D_80BE1970[] = { 0x003A0100, 0x080E2AFF, 0x0C113A01, 0x100E2B00, 0x0C100000 };

s32 D_80BE1984[] = { 0x00500100, 0x080E2B3F, 0x0C115001, 0x100E2B40, 0x0C100000 };

s32 D_80BE1998[] = { 0x0E2B090C, 0x10000000 };

s32 D_80BE19A0[] = {
    0x0E2B0A0C, 0x05004E00, 0x00004E30, 0x2800050E, 0x2B0E0C10, 0x0E2B0B0C, 0x05000000, 0x08001008, 0x00140026,
    0x19000E08, 0x00C8000F, 0x19000631, 0x0E2B0D0C, 0x10320E2B, 0x0C0C1030, 0x1214FF38, 0x06009100, 0x00130091,
    0x0C103012, 0x14FFEC06, 0x00920000, 0x1300920C, 0x10300034, 0x0100050E, 0x2B0F0C10, 0x0E2B110C, 0x10000000,
};

s32 D_80BE1A0C[] = {
    0x0E2B0A0C, 0x05004E00, 0x00004E30, 0x2800050E, 0x2B0E0C10, 0x0E2B0B0C, 0x05000000, 0x08001008, 0x00140026,
    0x19000E08, 0x00C8000F, 0x19000631, 0x0E2B0D0C, 0x10320E2B, 0x0C0C1030, 0x1214FF38, 0x06009100, 0x00130091,
    0x0C103012, 0x14FFEC06, 0x00920000, 0x1300920C, 0x10300034, 0x0100050E, 0x2B100C10, 0x0E2B110C, 0x10000000,
};

ActorInit En_Tab_InitVars = {
    ACTOR_EN_TAB,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_TAB,
    sizeof(EnTab),
    (ActorFunc)EnTab_Init,
    (ActorFunc)EnTab_Destroy,
    (ActorFunc)EnTab_Update,
    (ActorFunc)EnTab_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_HIT1,
        AT_NONE,
        AC_NONE,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK1,
        { 0x00000000, 0x00, 0x00 },
        { 0x00000000, 0x00, 0x00 },
        TOUCH_NONE | TOUCH_SFX_NORMAL,
        BUMP_NONE,
        OCELEM_ON,
    },
    { 14, 62, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit2 sColChkInfoInit = { 0, 0, 0, 0, MASS_IMMOVABLE };

AnimationInfoS D_80BE1AD0[] = {
    { &object_tab_Anim_000758, 1.0f, 0, -1, 0, 0 },
    { &object_tab_Anim_0086AC, 1.0f, 0, -1, 0, 0 },
};

Vec3f D_80BE1AF0 = { -28.0f, -8.0f, -195.0f };

Vec3s D_80BE1AFC = { 0, 0, 0 };

Vec3f D_80BE1B04 = { 161.0f, 0.0f, -10.0f };

Vec3s D_80BE1B10 = { 0, 0xC000, 0 };

Vec3f D_80BE1B18 = { 800.0f, 0.0f, 0.0f };

TexturePtr D_80BE1B24[] = {
    object_tab_Tex_006428,
    object_tab_Tex_006928,
    object_tab_Tex_006D28,
    object_tab_Tex_006928,
};

EnGm* func_80BE04E0(EnTab* this, PlayState* play, u8 actorCat, s16 actorId) {
    Actor* foundActor = NULL;
    Actor* tempActor;

    while (true) {
        foundActor = SubS_FindActor(play, foundActor, actorCat, actorId);
        if ((foundActor == NULL) || (((EnTab*)foundActor != this) && (foundActor->update != NULL))) {
            break;
        }

        tempActor = foundActor->next;
        if (tempActor == NULL) {
            foundActor = NULL;
            break;
        }
        foundActor = tempActor;
    };

    return (EnGm*)foundActor;
}

void func_80BE0590(EnTab* this) {
    this->skelAnime.playSpeed = this->unk_300;
    SkelAnime_Update(&this->skelAnime);
}

s32 func_80BE05BC(EnTab* this, s32 arg1) {
    s32 phi_v0 = false;
    s32 ret = false;

    if (arg1 != this->unk_32C) {
        phi_v0 = true;
    }

    if (phi_v0) {
        this->unk_32C = arg1;
        ret = SubS_ChangeAnimationByInfoS(&this->skelAnime, D_80BE1AD0, arg1);
        this->unk_300 = this->skelAnime.playSpeed;
    }
    return ret;
}

void func_80BE0620(EnTab* this, PlayState* play) {
    s32 pad;

    Collider_UpdateCylinder(&this->actor, &this->collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
}

void func_80BE0664(EnTab* this) {
    if (DECR(this->unk_31C) == 0) {
        this->unk_31E++;
        if (this->unk_31E > 3) {
            this->unk_31C = Rand_S16Offset(30, 30);
            this->unk_31E = 0;
        }
    }
}

s32 func_80BE06DC(EnTab* this, PlayState* play) {
    s32 ret = false;

    if (this->unk_2FC & 7) {
        if (Actor_ProcessTalkRequest(&this->actor, &play->state)) {
            SubS_UpdateFlags(&this->unk_2FC, 0, 7);
            ret = true;
            this->unk_320 = 0;
            this->unk_328 = NULL;
            this->actor.child = &GET_PLAYER(play)->actor;
            this->unk_2FC |= 8;
            this->actionFunc = func_80BE1348;
        }
    }
    return ret;
}

Actor* func_80BE0778(EnTab* this, PlayState* play) {
    Actor* actor;

    if (this->unk_1D8 == 1) {
        actor = this->actor.child;
    } else {
        actor = &GET_PLAYER(play)->actor;
    }
    return actor;
}

void func_80BE07A0(EnTab* this) {
    s32 pad;
    Vec3f sp40;
    Vec3f sp34;
    s16 sp32;

    Math_Vec3f_Copy(&sp40, &this->unk_1E0->world.pos);
    Math_Vec3f_Copy(&sp34, &this->actor.world.pos);
    sp32 = Math_Vec3f_Yaw(&sp34, &sp40);

    Math_ApproachS(&this->unk_314, (sp32 - this->unk_318) - this->actor.shape.rot.y, 4, 0x2AA8);
    this->unk_314 = CLAMP(this->unk_314, -0x1FFE, 0x1FFE);

    Math_ApproachS(&this->unk_318, (sp32 - this->unk_314) - this->actor.shape.rot.y, 4, 0x2AA8);
    this->unk_318 = CLAMP(this->unk_318, -0x1C70, 0x1C70);

    Math_Vec3f_Copy(&sp34, &this->actor.focus.pos);
    if (this->unk_1E0->id == ACTOR_PLAYER) {
        sp40.y = ((Player*)this->unk_1E0)->bodyPartsPos[7].y + 3.0f;
    } else {
        Math_Vec3f_Copy(&sp40, &this->unk_1E0->focus.pos);
    }

    Math_ApproachS(&this->unk_312, Math_Vec3f_Pitch(&sp34, &sp40) - this->unk_316, 4, 0x2AA8);
    this->unk_312 = CLAMP(this->unk_312, -0x1554, 0x1554);

    Math_ApproachS(&this->unk_316, Math_Vec3f_Pitch(&sp34, &sp40) - this->unk_312, 4, 0x2AA8);
    this->unk_316 = CLAMP(this->unk_316, -0xE38, 0xE38);
}

void func_80BE09A8(EnTab* this, PlayState* play) {
    this->unk_1E0 = func_80BE0778(this, play);
    if ((this->unk_2FC & 8) && (this->unk_1E0 != 0) && (DECR(this->unk_324) == 0)) {
        func_80BE07A0(this);
        this->unk_316 = 0;
        this->unk_318 = 0;
        this->unk_2FC &= ~0x40;
        this->unk_2FC |= 0x10;
    } else if (this->unk_2FC & 0x10) {
        this->unk_2FC &= ~0x10;
        this->unk_312 = 0;
        this->unk_314 = 0;
        this->unk_316 = 0;
        this->unk_318 = 0;
        this->unk_324 = 20;
    } else if (DECR(this->unk_324) == 0) {
        this->unk_2FC |= 0x40;
    }
}

void func_80BE0A98(EnTab* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 talkState = Message_GetState(&play->msgCtx);

    this->unk_308 += (this->unk_304 != 0.0f) ? 40.0f : -40.0f;
    this->unk_308 = CLAMP(this->unk_308, 0.0f, 80.0f);

    Matrix_Translate(this->unk_308, 0.0f, 0.0f, MTXMODE_APPLY);

    if ((&this->actor == player->talkActor) &&
        ((play->msgCtx.currentTextId < 0xFF) || (play->msgCtx.currentTextId > 0x200)) && (talkState == TEXT_STATE_3) &&
        (this->prevTalkState == TEXT_STATE_3)) {
        if ((play->state.frames % 2) == 0) {
            if (this->unk_304 != 0.0f) {
                this->unk_304 = 0.0f;
            } else {
                this->unk_304 = 1.0f;
            }
        }
    } else {
        this->unk_304 = 0.0f;
    }
    this->prevTalkState = talkState;
}

s32 func_80BE0C04(EnTab* this, Actor* actor, f32 arg2) {
    Vec3f sp44;
    Vec3f sp38;
    s32 ret = false;
    f32 sp30;
    s16 sp2E;

    if (actor != NULL) {
        Math_Vec3f_Copy(&sp38, &this->actor.world.pos);
        Math_Vec3f_Copy(&sp44, &actor->world.pos);
        sp2E = Math_Vec3f_Yaw(&sp38, &sp44);

        if (this->unk_338) {
            sp30 = arg2;
        } else {
            sp30 = arg2 * 0.5f;
        }
        if ((Math_Vec3f_DistXZ(&sp44, &sp38) < sp30) && (ABS_ALT(BINANG_SUB(sp2E, this->actor.shape.rot.y)) < 0x38E0)) {
            ret = true;
        }

        if (DECR(this->unk_322) == 0) {
            this->unk_338 ^= true;
            this->unk_322 = Rand_S16Offset(60, 60);
        }
    }
    return ret;
}

s32 func_80BE0D38(EnTab* this, PlayState* play) {
    return Inventory_HasEmptyBottle();
}

s32 func_80BE0D60(EnTab* this, PlayState* play) {
    s32 pad;
    s32 ret = false;

    this->unk_320++;
    if (this->unk_320 == 1) {
        play->setPlayerTalkAnim(play, &gPlayerAnim_link_demo_bikkuri, ANIMMODE_ONCE);
    } else if (this->unk_320 > 20) {
        play->setPlayerTalkAnim(play, NULL, ANIMMODE_LOOP);
        this->unk_320 = 0;
        ret = true;
    }
    return ret;
}

s32* func_80BE0E04(EnTab* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (player->transformation == PLAYER_FORM_DEKU) {
        return D_80BE1984;
    }

    switch (this->unk_1D8) {
        case 2:
            this->unk_328 = func_80BE0D38;
            if (Player_GetMask(play) != PLAYER_MASK_ROMANI) {
                return D_80BE1998;
            }

            if (gSaveContext.save.day == 3) {
                return D_80BE1A0C;
            }
            return D_80BE19A0;

        case 1:
            this->unk_328 = func_80BE0D60;
            if (Player_GetMask(play) == PLAYER_MASK_ROMANI) {
                return D_80BE1940;
            }

            if (Player_GetMask(play) == PLAYER_MASK_KAFEIS_MASK) {
                return D_80BE1970;
            }
            return D_80BE1914;
    }

    return NULL;
}

s32 func_80BE0F04(EnTab* this, PlayState* play, ScheduleOutput* scheduleOutput) {
    s32 ret = false;
    EnGm* sp28 = func_80BE04E0(this, play, ACTORCAT_NPC, ACTOR_EN_GM);

    if (sp28) {
        Math_Vec3f_Copy(&this->actor.world.pos, &D_80BE1AF0);
        Math_Vec3s_Copy(&this->actor.world.rot, &D_80BE1AFC);
        Math_Vec3s_Copy(&this->actor.shape.rot, &this->actor.world.rot);
        this->actor.targetMode = 0;
        SubS_UpdateFlags(&this->unk_2FC, 3, 7);
        this->unk_2FC |= (0x40 | 0x20);
        this->unk_30C = 30;
        this->unk_1E4 = sp28;
        func_80BE05BC(this, 0);
        ret = true;
    }
    return ret;
}

s32 func_80BE0FC4(EnTab* this, PlayState* play, ScheduleOutput* scheduleOutput) {
    s32 pad;

    Math_Vec3f_Copy(&this->actor.world.pos, &D_80BE1B04);
    Math_Vec3s_Copy(&this->actor.world.rot, &D_80BE1B10);
    Math_Vec3s_Copy(&this->actor.shape.rot, &this->actor.world.rot);
    this->actor.targetMode = 6;
    SubS_UpdateFlags(&this->unk_2FC, 3, 7);
    this->unk_2FC |= (0x40 | 0x20);
    this->unk_30C = 0x50;
    func_80BE05BC(this, 1);
    return true;
}

s32 func_80BE1060(EnTab* this, PlayState* play, ScheduleOutput* scheduleOutput) {
    s32 ret;

    this->unk_2FC = 0;

    switch (scheduleOutput->result) {
        case 1:
            ret = func_80BE0F04(this, play, scheduleOutput);
            break;

        case 2:
            ret = func_80BE0FC4(this, play, scheduleOutput);
            break;

        default:
            ret = false;
            break;
    }
    return ret;
}

s32 func_80BE10BC(EnTab* this, PlayState* play) {
    s32 pad;
    Player* player = GET_PLAYER(play);
    f32 dist;
    Actor* tempActor;

    switch (this->unk_1D8) {
        case 1:
            if ((player->stateFlags1 & PLAYER_STATE1_40) && !(play->msgCtx.currentTextId <= 0x2B00) &&
                (play->msgCtx.currentTextId < 0x2B08)) {
                this->actor.child = &this->unk_1E4->actor;
                this->unk_2FC |= 8;
            } else {
                dist = Math_Vec3f_DistXYZ(&this->actor.world.pos, &this->unk_1E4->actor.world.pos);

                if (CHECK_WEEKEVENTREG(WEEKEVENTREG_75_01) || (this->unk_1E4->actor.draw == NULL) || !(dist < 160.0f)) {
                    tempActor = &GET_PLAYER(play)->actor;
                    this->actor.child = tempActor;
                } else {
                    tempActor = &this->unk_1E4->actor;
                    this->actor.child = tempActor;
                }

                if (func_80BE0C04(this, tempActor, 160.0f)) {
                    this->unk_2FC |= 8;
                } else {
                    this->unk_2FC &= ~8;
                }
            }
            break;

        case 2:
            if (func_80BE0C04(this, &GET_PLAYER(play)->actor, 200.0f)) {
                this->unk_2FC |= 8;
            } else {
                this->unk_2FC &= ~8;
            }
            break;
    }

    return false;
}

void func_80BE1224(EnTab* this, PlayState* play) {
    if ((this->unk_1D8 == 1) || (this->unk_1D8 == 2)) {
        func_80BE10BC(this, play);
    }
    Math_ApproachS(&this->actor.shape.rot.y, this->actor.world.rot.y, 3, 0x2AA8);
}

void func_80BE127C(EnTab* this, PlayState* play) {
    ScheduleOutput sp18;

    this->unk_31A = R_TIME_SPEED + ((void)0, gSaveContext.save.timeSpeedOffset);

    if (!Schedule_RunScript(play, D_80BE18D0, &sp18) ||
        ((this->unk_1D8 != sp18.result) && !func_80BE1060(this, play, &sp18))) {
        this->actor.shape.shadowDraw = NULL;
        this->actor.flags &= ~ACTOR_FLAG_1;
        sp18.result = 0;
    } else {
        this->actor.shape.shadowDraw = ActorShadow_DrawCircle;
        this->actor.flags |= ACTOR_FLAG_1;
    }
    this->unk_1D8 = sp18.result;
    func_80BE1224(this, play);
}

void func_80BE1348(EnTab* this, PlayState* play) {
    s32 pad;
    Vec3f sp40;
    Vec3f sp34;

    if (func_8010BF58(&this->actor, play, func_80BE0E04(this, play), this->unk_328, &this->unk_1DC)) {
        SubS_UpdateFlags(&this->unk_2FC, 3, 7);
        this->unk_2FC &= ~8;
        this->unk_2FC |= 0x40;
        this->unk_324 = 20;
        this->unk_1DC = 0;
        this->actionFunc = func_80BE127C;
    } else if (this->unk_1E0 != 0) {
        Math_Vec3f_Copy(&sp40, &this->unk_1E0->world.pos);
        Math_Vec3f_Copy(&sp34, &this->actor.world.pos);
        Math_ApproachS(&this->actor.shape.rot.y, Math_Vec3f_Yaw(&sp34, &sp40), 4, 0x2AA8);
    }
}

void EnTab_Init(Actor* thisx, PlayState* play) {
    EnTab* this = THIS;

    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 14.0f);
    SkelAnime_InitFlex(play, &this->skelAnime, &object_tab_Skel_007F78, NULL, this->jointTable, this->morphTable, 20);
    this->unk_32C = -1;
    func_80BE05BC(this, 0);
    Collider_InitAndSetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
    CollisionCheck_SetInfo2(&this->actor.colChkInfo, DamageTable_Get(0x16), &sColChkInfoInit);
    Actor_SetScale(&this->actor, 0.01f);

    this->unk_1D8 = 0;
    this->unk_328 = NULL;
    this->unk_2FC = 0;
    this->unk_2FC |= 0x40;
    this->actor.gravity = -1.0f;

    this->actionFunc = func_80BE127C;
    this->actionFunc(this, play);
}

void EnTab_Destroy(Actor* thisx, PlayState* play) {
    EnTab* this = THIS;

    Collider_DestroyCylinder(play, &this->collider);
}

void EnTab_Update(Actor* thisx, PlayState* play) {
    EnTab* this = THIS;
    f32 radius;
    f32 height;

    func_80BE06DC(this, play);

    this->actionFunc(this, play);

    if (this->unk_1D8 != 0) {
        func_80BE0590(this);
        func_80BE0664(this);
        func_80BE09A8(this, play);

        radius = this->collider.dim.radius + this->unk_30C;
        height = this->collider.dim.height + 10;

        func_8013C964(&this->actor, play, radius, height, PLAYER_IA_NONE, this->unk_2FC & 7);
        Actor_MoveWithGravity(&this->actor);
        Actor_UpdateBgCheckInfo(play, &this->actor, 30.0f, 12.0f, 0.0f, UPDBGCHECKINFO_FLAG_4);
        func_80BE0620(this, play);
    }
}

s32 EnTab_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx) {
    EnTab* this = THIS;

    if (limbIndex == 9) {
        func_80BE0A98(this, play);
    }

    if ((this->unk_32C == 1) && (limbIndex == 18)) {
        *dList = NULL;
    }
    return false;
}

void EnTab_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor* thisx) {
    EnTab* this = THIS;

    if (limbIndex == 9) {
        Matrix_MultVec3f(&D_80BE1B18, &this->actor.focus.pos);
        Math_Vec3s_Copy(&this->actor.focus.rot, &this->actor.world.rot);
    }
}

void EnTab_TransformLimbDraw(PlayState* play, s32 limbIndex, Actor* thisx) {
    EnTab* this = THIS;
    s32 rotStep;
    s32 overrideStep;

    if (!(this->unk_2FC & 0x40)) {
        if (this->unk_2FC & 0x10) {
            overrideStep = true;
        } else {
            overrideStep = false;
        }
        rotStep = true;
    } else {
        overrideStep = false;
        rotStep = false;
    }

    if (limbIndex == 9) {
        SubS_UpdateLimb(BINANG_ADD(this->unk_312 + this->unk_316, 0x4000),
                        BINANG_ADD(this->unk_314 + this->unk_318 + this->actor.shape.rot.y, 0x4000), this->unk_1E8,
                        this->unk_200, rotStep, overrideStep);
        Matrix_Pop();
        Matrix_Translate(this->unk_1E8[0].x, this->unk_1E8[0].y, this->unk_1E8[0].z, MTXMODE_NEW);
        Matrix_Scale(this->actor.scale.x, this->actor.scale.y, this->actor.scale.z, MTXMODE_APPLY);
        Matrix_RotateYS(this->unk_200[0].y, MTXMODE_APPLY);
        Matrix_RotateXS(this->unk_200[0].x, MTXMODE_APPLY);
        Matrix_RotateZS(this->unk_200[0].z, MTXMODE_APPLY);
        Matrix_Push();
    }
}

void EnTab_Draw(Actor* thisx, PlayState* play) {
    EnTab* this = THIS;

    if (this->unk_1D8 != 0) {
        OPEN_DISPS(play->state.gfxCtx);

        Gfx_SetupDL25_Opa(play->state.gfxCtx);

        gSPSegment(POLY_OPA_DISP++, 0x08, Lib_SegmentedToVirtual(D_80BE1B24[this->unk_31E]));

        SkelAnime_DrawTransformFlexOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable,
                                       this->skelAnime.dListCount, EnTab_OverrideLimbDraw, EnTab_PostLimbDraw,
                                       EnTab_TransformLimbDraw, &this->actor);

        CLOSE_DISPS(play->state.gfxCtx);
    }
}
